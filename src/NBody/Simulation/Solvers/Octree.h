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
        using NodeBase<OctreeNode>::averagePosition;

        [[nodiscard]] Position &center() { return _center; }

        [[nodiscard]] const Position &center() const { return _center; }

        [[nodiscard]] const float &sideLength() const { return _sideLength; }

        [[nodiscard]] float &sideLength() { return _sideLength; }

        std::vector<OctreeNode> &children() { return _children; }

        const std::vector<OctreeNode> &children() const { return _children; }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

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
    public:

        Octree(Simulation &simulation) : TreeBase<OctreeNode>(simulation) {}

        void build() override;
    };
}

/*
namespace NBody {

    class Octree {
    private:

        class OctreeNode {
        private:

            std::unique_ptr<std::array<OctreeNode, 8>> _children;
            std::span<NBody::Entity> _contents;

            Physics::Position _center;
            float _sideLength = 1.0f;

            Physics::Position _centerOfMass;
            Physics::Mass _totalMass;

        public:

            // todo Maybe this can be removed?
            OctreeNode() = default;

            // Recursive constructor is gross, this will be replaced
            OctreeNode(std::span<NBody::Entity> contents, const NBody::Simulation &registry,
                       Physics::Position center, float sideLength, int maxDepth, int maxLeafSize);


            [[nodiscard]] float sideLength() const { return _sideLength; }

            [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

            [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

            [[nodiscard]] bool isLeaf() const { return !_children; }

            [[nodiscard]] const std::array<OctreeNode, 8> &children() const { return *_children; }

            [[nodiscard]] const std::span<NBody::Entity> &particles() const { return _contents; }

            [[nodiscard]] Physics::Acceleration applyRule(const Physics::Rule &rule, Simulation &simulation,
                                                          const Physics::Position &passivePosition,
                                                          const Physics::Mass &passiveMass,
                                                          float theta) const {

                // Empty nodes can be ignored
                if (_contents.empty()) return Physics::Acceleration{};

                if ((_sideLength / glm::distance((glm::vec3) passivePosition, (glm::vec3) centerOfMass())) < theta) {

                    // Node is treated as a single particle if S/D < theta (where S = sideLength and D = distance)
                    return rule(centerOfMass(),
                                totalMass(),
                                passivePosition, passiveMass);

                } else {

                    // Otherwise, the node can't be summarized
                    if (isLeaf()) {

                        auto actors = simulation.group<const Position, const Mass>(entt::get<const ActiveTag>);

                        // If this is a leaf node, interact with all particles contained
                        return std::transform_reduce(
                                _contents.begin(), _contents.end(),
                                Physics::Acceleration{}, std::plus{},
                                [&](auto entity) {
                                    return rule(actors.get<const Position>(entity),
                                                actors.get<const Mass>(entity),
                                                passivePosition, passiveMass);
                                });

                    } else {

                        // If it's a non-leaf node, descend the tree (recursive case)
                        return std::transform_reduce(
                                children().begin(), children().end(),
                                Physics::Acceleration{}, std::plus{},
                                [&](const auto &child) {
                                    return child.applyRule(rule, simulation, passivePosition, passiveMass, theta);
                                });
                    }

                }
            }

            std::string toString(const std::string &padding = "") {
                std::stringstream out;
                out << padding << std::to_string(_contents.size());

                if (_children)
                    for (auto &child: *_children)
                        out << "\n" << child.toString(padding + "   ");

                return out.str();
            }

        };

    private:

        NBody::Simulation &_simulation;

        OctreeNode _root;

        std::vector<NBody::Entity> _particles;

    public:

        explicit Octree(NBody::Simulation &simulation) : _simulation(simulation) {

            // Save a list of physics-actor particles
            auto actors = _simulation.group<const Position, const Mass>(entt::get<ActiveTag>);
            _particles = {actors.begin(), actors.end()};

            // If the simulation has new particles, add them to the list
            _simulation.signal_particles_added.connect([&](auto newEntities) {
                auto actors = _simulation.group<const Position, const Mass>(entt::get<ActiveTag>);
                _particles = {actors.begin(), actors.end()};
            });

            // If the simulation has particles removed, take them from the list
            _simulation.signal_particles_removed.connect([&](auto removedEntities) {
                auto actors = _simulation.group<const Position, const Mass>(entt::get<ActiveTag>);
                _particles = {actors.begin(), actors.end()};
            });
        }

        void build(int maxDepth, int maxLeafSize) {
            _root = OctreeNode{
                    {_particles}, _simulation, {}, (2 << 20),
                    maxDepth, maxLeafSize
            };
        }

        Physics::Acceleration applyRule(const Physics::Rule &rule,
                                        const Physics::Position &passivePosition,
                                        const Physics::Mass &passiveMass,
                                        float theta) {

            return _root.applyRule(rule, _simulation, passivePosition, passiveMass, theta);

            Physics::Acceleration acceleration;

            std::stack<const OctreeNode *> nodes;
            nodes.emplace(&_root);

            while (!nodes.empty()) {

                const auto &n = *nodes.top();
                nodes.pop();

                float sideLengthOverDistance =
                        n.sideLength() / glm::distance((glm::vec3) passivePosition, (glm::vec3) n.centerOfMass());
                if (sideLengthOverDistance < theta) {

                    // Base case 1: node is sufficiently far
                    acceleration += rule(n.centerOfMass(), n.totalMass(), passivePosition, passiveMass);

                } else if (n.isLeaf()) {

                    // Base case 2: we've reached a leaf node
                    acceleration += std::transform_reduce(
                            n.particles().begin(), n.particles().end(),
                            Physics::Acceleration{}, std::plus{},
                            [&](auto entity) {
                                return rule(_simulation.get<Position>(entity),
                                            _simulation.get<Mass>(entity),
                                            passivePosition, passiveMass);
                            });

                } else {

                    // Recursive case: the node isn't far enough, and we need to look at its children
                    for (const auto &child: n.children())
                        nodes.emplace(&child);
                }
            }

            return acceleration;
        }


    };

}
*/

#endif //N_BODY_OCTREE_H
