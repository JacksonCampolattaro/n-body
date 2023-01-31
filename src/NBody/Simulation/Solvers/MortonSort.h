//
// Created by Jackson Campolattaro on 1/9/23.
//

#ifndef N_BODY_MORTONSORT_H
#define N_BODY_MORTONSORT_H

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

    static void mortonSort(const entt::basic_view<entt::entity, entt::exclude_t<>, const MortonCode> &mortonCodes,
                           std::vector<Entity> &indices) {

        boost::sort::spreadsort::integer_sort(indices.begin(), indices.end(),
                                              [&](Entity i, unsigned offset) {
                                                  assert(mortonCodes.contains(i));
                                                  return mortonCodes.get<const MortonCode>(i) >> offset;
                                              },
                                              [&](Entity a, Entity b) {
                                                  return mortonCodes.get<const MortonCode>(a) <
                                                         mortonCodes.get<const MortonCode>(b);
                                              }
        );
    }


}
#endif //N_BODY_MORTONSORT_H
