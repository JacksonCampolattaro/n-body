//
// Created by Jackson Campolattaro on 7/3/22.
//

#ifndef N_BODY_OCTREE_H
#define N_BODY_OCTREE_H

#include <span>
#include <set>
#include <stack>

#include <spdlog/spdlog.h>

#include "Tree.h"

#include <NBody/Simulation/Simulation.h>

#include "NBody/Physics/Rule.h"

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::Mass;
using NBody::Physics::ActiveTag;

namespace {

    template<int Dimension>
    [[nodiscard]] auto
    partition(std::span<NBody::Entity> &entities,
              const entt::basic_view<entt::entity, entt::exclude_t<>,
                      const Position, const Mass, const ActiveTag> &activeParticles,
              const NBody::Physics::Position &center) {

        auto c = std::partition(entities.begin(), entities.end(), [&](NBody::Entity entity) {
            return (activeParticles.get<const Position>(entity))[Dimension] < center[Dimension];
        });

        std::span less{&*entities.begin(), static_cast<std::size_t>(c - entities.begin())};
        std::span more{&*c, static_cast<std::size_t>(entities.end() - c)};
        assert(less.size() + more.size() == entities.size());

        // This can be done with std::arrays, using something like:
        // https://stackoverflow.com/questions/10604794/convert-stdtuple-to-stdarray-c11
        // Or this:
        // https://stackoverflow.com/questions/45287195/combine-two-or-more-arrays-of-different-size-to-one-array-at-compiletime
        return std::tuple_cat(partition<Dimension - 1>(less, activeParticles, center),
                              partition<Dimension - 1>(more, activeParticles, center));
    }

    template<>
    [[nodiscard]] auto
    partition<0>(std::span<NBody::Entity> &entities,
                 const entt::basic_view<entt::entity, entt::exclude_t<>,
                         const Position, const Mass, const ActiveTag> &activeParticles,
                 const NBody::Physics::Position &center) {

        auto c = std::partition(entities.begin(), entities.end(), [&](NBody::Entity entity) {
            return (activeParticles.get<const Position>(entity))[0] < center[0];
        });

        std::span less{&*entities.begin(), static_cast<std::size_t>(c - entities.begin())};
        std::span more{&*c, static_cast<std::size_t>(entities.end() - c)};
        assert(less.size() + more.size() == entities.size());

        return std::tuple{less, more};
    }

}

namespace NBody {

    class OctreeNode : public NodeBase<OctreeNode> {
    private:

        std::vector<OctreeNode> _children;

        Position _center;
        float _sideLength;

        Mass _totalMass;
        Position _centerOfMass;

    public:

        OctreeNode(std::span<Entity> contents,
                   const Position &center = {0.0f, 0.0f, 0.0f},
                   float sideLength = 10000.0f) :
                NodeBase<OctreeNode>(contents),
                _center(center),
                _sideLength(sideLength) {}

        using NodeBase<OctreeNode>::contents;
        using NodeBase<OctreeNode>::isLeaf;

        [[nodiscard]] std::vector<OctreeNode> &children() { return _children; }

        [[nodiscard]] const std::vector<OctreeNode> &children() const { return _children; }

        [[nodiscard]] BoundingBox boundingBox() const {
            glm::vec3 dimensions{_sideLength, _sideLength, _sideLength};
            return {
                    {_center - dimensions},
                    {_center + dimensions}
            };
        }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

        [[nodiscard]] Position &center() { return _center; }

        [[nodiscard]] const Position &center() const { return _center; }

        [[nodiscard]] const float &sideLength() const { return _sideLength; }

        [[nodiscard]] float &sideLength() { return _sideLength; }

        void split(const entt::basic_view<entt::entity, entt::exclude_t<>,
                const Position, const Mass, const ActiveTag> &activeParticles) {

            std::span<NBody::Entity>
                    xyz000,
                    xyz100,
                    xyz010,
                    xyz110,
                    xyz001,
                    xyz101,
                    xyz011,
                    xyz111;

            std::tie(
                    xyz000,
                    xyz100,
                    xyz010,
                    xyz110,
                    xyz001,
                    xyz101,
                    xyz011,
                    xyz111
            ) = partition<2>(contents(), activeParticles, _center);

            // Initialize 8 child nodes
            float childSideLength = sideLength() / 2.0f;
            _children = {{
                                 {xyz000,
                                  center() + glm::vec3{-childSideLength, -childSideLength, -childSideLength},
                                  childSideLength},
                                 {xyz001,
                                  center() + glm::vec3{-childSideLength, -childSideLength, childSideLength},
                                  childSideLength},
                                 {xyz010,
                                  center() + glm::vec3{-childSideLength, childSideLength, -childSideLength},
                                  childSideLength},
                                 {xyz011,
                                  center() + glm::vec3{-childSideLength, childSideLength, childSideLength},
                                  childSideLength},
                                 {xyz100,
                                  center() + glm::vec3{childSideLength, -childSideLength, -childSideLength},
                                  childSideLength},
                                 {xyz101,
                                  center() + glm::vec3{childSideLength, -childSideLength, childSideLength},
                                  childSideLength},
                                 {xyz110,
                                  center() + glm::vec3{childSideLength, childSideLength, -childSideLength},
                                  childSideLength},
                                 {xyz111,
                                  center() + glm::vec3{childSideLength, childSideLength, childSideLength},
                                  childSideLength}
                         }};
        }


        void summarize(const entt::basic_view<entt::entity, entt::exclude_t<>,
                const Position, const Mass, const ActiveTag> &activeParticles) {

            _totalMass = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};

            if (isLeaf()) {

                for (const auto &entity: contents()) {
                    auto entityMass = activeParticles.get<const Mass>(entity).mass();
                    _totalMass.mass() += entityMass;
                    _centerOfMass = _centerOfMass + (entityMass * activeParticles.get<const Position>(entity));
                }
                _centerOfMass = _centerOfMass / _totalMass.mass();

            } else {

                for (const auto &child: children()) {
                    _totalMass.mass() += child.totalMass().mass();
                    _centerOfMass = _centerOfMass + (child.centerOfMass() * child.totalMass().mass());
                }
                _centerOfMass = _centerOfMass / _totalMass.mass();

            }
        }

        void merge() {
            _children.clear();
        }

    };

    class Octree : public TreeBase<OctreeNode> {
    private:

        int _maxDepth = 16;
        int _maxLeafSize = 64;

    public:

        Octree(Simulation &simulation) : TreeBase<OctreeNode>(simulation) {}

        void build() override {

            BoundingBox boundingBox = simulation().boundingBox();
            glm::vec3 dimensions = boundingBox.dimensions();
            root().center() = (boundingBox.max() - boundingBox.min()) / 2.0f;
            root().sideLength() = std::max(std::max(dimensions.x, dimensions.y), dimensions.z);

            root().refine(
                    _maxDepth,
                    [&](const auto &n) { return n.contents().size() > _maxLeafSize; },
                    simulation().view<const Position, const Mass, const ActiveTag>()
            );
        };

        int &maxDepth() { return _maxDepth; }

        const int &maxDepth() const { return _maxDepth; }

        int &maxLeafSize() { return _maxLeafSize; }

        const int &maxLeafSize() const { return _maxLeafSize; }
    };
}

#endif //N_BODY_OCTREE_H
