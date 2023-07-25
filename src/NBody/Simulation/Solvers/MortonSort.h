//
// Created by Jackson Campolattaro on 1/9/23.
//

#ifndef N_BODY_MORTONSORT_H
#define N_BODY_MORTONSORT_H

#include <tbb/parallel_for_each.h>

#include <cstdint>
#include <algorithm>

#include <boost/sort/spreadsort/spreadsort.hpp>

#include <glm/glm.hpp>

#include <NBody/Simulation/Simulation.h>

// adapted from
// https://github.com/Markvanderuit/dual_hierarchy_tsne/blob/6247abb752dd5967c370c370460d71012bfad635/resources/shaders/sne/embedding_hierarchy/3D/mortonUnsorted.comp

namespace NBody {

    using MortonCode = std::uint32_t;

    static std::uint32_t expandBits10(std::uint32_t i) {
        i = (i * 0x00010001u) & 0xFF0000FFu;
        i = (i * 0x00000101u) & 0x0F00F00Fu;
        i = (i * 0x00000011u) & 0xC30C30C3u;
        i = (i * 0x00000005u) & 0x49249249u;
        return i;
    }

    static MortonCode toMortonCode(glm::vec3 v) {
        v = glm::clamp(v * 1024.f, 0.f, 1023.f);
        std::uint32_t x = expandBits10(std::uint32_t(v.x));
        std::uint32_t y = expandBits10(std::uint32_t(v.y));
        std::uint32_t z = expandBits10(std::uint32_t(v.z));
        return x * 4u + y * 2u + z;
    }


    static void setMortonCodes(Simulation &simulation, BoundingBox boundingBox) {

        glm::vec3 dimensions = boundingBox.dimensions();
        float scale = std::max(std::max(dimensions.x, dimensions.y), dimensions.z);

        auto activeParticles = simulation.group<const Position, const Mass>();
        activeParticles.each([&](const auto e, const auto &position, const Mass) {
            // Each position should be normalized to [0, 1] before conversion
            simulation.emplace_or_replace<MortonCode>(e, toMortonCode((position - boundingBox.min()) / scale));
        });
    }

    template<typename MortonCodeView>
    static void mortonSort(const MortonCodeView &mortonCodes, std::span<Entity> indices) {

        boost::sort::spreadsort::integer_sort(indices.begin(), indices.end(),
                                              [&](Entity i, unsigned offset) {
                                                  assert(mortonCodes.contains(i));
                                                  return mortonCodes.template get<const MortonCode>(i) >> offset;
                                              },
                                              [&](Entity a, Entity b) {
                                                  return mortonCodes.template get<const MortonCode>(a) <
                                                         mortonCodes.template get<const MortonCode>(b);
                                              }
        );
    }

    template<typename MortonCodeView>
    static void parallelMortonSort(
            const MortonCodeView &mortonCodes,
            std::span<Entity> indices, std::size_t threads = 12
    ) {

        std::vector<std::span<Entity>> queue;
        auto comparator = [&](auto &a, auto &b) {
            return a.size() < b.size();
        };

        queue.push_back(indices);

        // todo: this could be done in parallel, too!
        while (queue.size() < threads) {

            std::pop_heap(queue.begin(), queue.end(), comparator);
            std::span<Entity> range = queue.back();
            queue.pop_back();

            // Determine the highest different bit
            // I wish I could think of a less stupid way to do this...
            int msb = 0;
            for (int i = 1; i < range.size(); ++i) {
                msb = std::max(
                        msb,
                        (8 * sizeof(MortonCode)) -
                        std::countl_zero(mortonCodes.template get<const MortonCode>(range[i - 1]) ^
                                         mortonCodes.template get<const MortonCode>(range[i])) - 1
                );
            }

            // Partition the range based on that bit
            MortonCode boundary = (1 << msb);
            auto middle = std::partition(range.begin(), range.end(), [&](auto e) -> bool {
                return (mortonCodes.template get<const MortonCode>(e) & boundary) == 0;
            });

            queue.emplace_back(range.begin(), middle);
            std::push_heap(queue.begin(), queue.end(), comparator);

            queue.emplace_back(middle, range.end());
            std::push_heap(queue.begin(), queue.end(), comparator);
        }

        // Each range can now be radix-sorted independently
        tbb::parallel_for_each(queue, [&](std::span<Entity> range) {
            mortonSort(mortonCodes, range);
        });
    }

    template<typename MortonCodeView>
    static void recursiveParallelMortonSort(
            const MortonCodeView &mortonCodes,
            std::span<Entity> indices, std::size_t threads = 12
    ) {

        if (threads > 1) {

            // Determine the highest different bit
            // I wish I could think of a less stupid way to do this...
            int msb = 0;
            for (int i = 1; i < indices.size(); ++i) {
                msb = std::max(
                        msb,
                        (8 * sizeof(MortonCode)) -
                        std::countl_zero(mortonCodes.template get<const MortonCode>(indices[i - 1]) ^
                                         mortonCodes.template get<const MortonCode>(indices[i])) - 1
                );
            }

            // Partition the range based on that bit
            MortonCode boundary = (1 << msb);
            auto middle = std::partition(indices.begin(), indices.end(), [&](auto e) -> bool {
                return (mortonCodes.template get<const MortonCode>(e) & boundary) == 0;
            });

            // Each sub-range can now be sorted independently
            tbb::parallel_for_each(
                    std::vector<std::span<Entity>>{
                            {indices.begin(), middle},
                            {middle,          indices.end()}
                    },
                    [&](std::span<Entity> range) {
                        recursiveParallelMortonSort(mortonCodes, range, threads / 2);
                    }
            );

        } else {
            // Base case, use radix sort
            mortonSort(mortonCodes, indices);
        }
    }
}
#endif //N_BODY_MORTONSORT_H
