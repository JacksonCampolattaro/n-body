//
// Created by Jackson Campolattaro on 7/3/22.
//

#ifndef N_BODY_OCTREE_H
#define N_BODY_OCTREE_H

#include <NBody/Simulation/Simulation.h>

#include <span>
#include <set>
#include <stack>
#include "NBody/Physics/Rule.h"

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::Mass;
using NBody::Physics::ActiveTag;

namespace {

    template<int Dimension>
    [[nodiscard]] auto
    split(std::span<NBody::Entity> &entities,
          const NBody::Simulation &registry,
          const NBody::Physics::Position &center) {

        auto c = std::partition(entities.begin(), entities.end(), [&](NBody::Entity entity) {
            return (registry.template get<NBody::Physics::Position>(entity))[Dimension] < center[Dimension];
        });

        std::span less{&*entities.begin(), static_cast<std::size_t>(c - entities.begin())};
        std::span more{&*c, static_cast<std::size_t>(entities.end() - c)};
        assert(less.size() + more.size() == entities.size());

        // This can be done with std::arrays, using something like:
        // https://stackoverflow.com/questions/10604794/convert-stdtuple-to-stdarray-c11
        // Or this:
        // https://stackoverflow.com/questions/45287195/combine-two-or-more-arrays-of-different-size-to-one-array-at-compiletime
        return std::tuple_cat(split<Dimension - 1>(less, registry, center),
                              split<Dimension - 1>(more, registry, center));
    }

    template<>
    [[nodiscard]] auto
    split<0>(std::span<NBody::Entity> &entities,
             const NBody::Simulation &registry,
                                 const NBody::Physics::Position &center) {

        auto c = std::partition(entities.begin(), entities.end(), [&](NBody::Entity entity) {
            return (registry.template get<NBody::Physics::Position>(entity))[0] < center[0];
        });

        std::span less{&*entities.begin(), static_cast<std::size_t>(c - entities.begin())};
        std::span more{&*c, static_cast<std::size_t>(entities.end() - c)};
        assert(less.size() + more.size() == entities.size());

        return std::tuple{less, more};
    }

}

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

        Octree(NBody::Simulation &simulation) : _simulation(
                simulation) {

            // Save a list of physics-actor particles
            auto actors = _simulation.group<const Position, const Mass>(entt::get<ActiveTag>);
            _particles = {actors.begin(), actors.end()};

            // If the simulation has new particles, add them to the list
            _simulation.signal_particles_added.connect([&](auto newEntities) {
                // todo: mark the octree root as invalid
            });

            // If the simulation has particles removed, take them from the list
            _simulation.signal_particle_removed.connect([&](auto removedEntity) {
                _particles.erase(std::find(_particles.begin(), _particles.end(), removedEntity));
                // todo: mark the octree root as invalid
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

#endif //N_BODY_OCTREE_H
