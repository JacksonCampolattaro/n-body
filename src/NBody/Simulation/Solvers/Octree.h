//
// Created by Jackson Campolattaro on 7/3/22.
//

#ifndef N_BODY_OCTREE_H
#define N_BODY_OCTREE_H

#include <NBody/Simulation/Simulation.h>

#include <span>
#include <set>
#include "NBody/Physics/Rule.h"

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::ActiveMass;
using NBody::Physics::PassiveMass;

namespace NBody {

    class Octree {
    private:

        class OctreeNode {
        private:

            std::unique_ptr<std::array<OctreeNode, 8>> _children;
            std::span<NBody::Entity> _contents;

            Physics::Position _center;
            float _sideLength;

            Physics::Position _centerOfMass;
            Physics::ActiveMass _totalMass;

        public:

            // todo Maybe this can be removed?
            OctreeNode() {}

            // Recursive constructor is gross, this will be replaced
            OctreeNode(std::span<NBody::Entity> contents, const NBody::Simulation &registry,
                       Physics::Position center = {}, float sideLength = (2 << 20));


            [[nodiscard]] float sideLength() const { return _sideLength; }

            [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

            [[nodiscard]] const ActiveMass &totalMass() const { return _totalMass; }

            [[nodiscard]] bool isLeaf() const { return !_children; }

            [[nodiscard]] const std::array<OctreeNode, 8> &children() const { return *_children; }

            [[nodiscard]] const std::span<NBody::Entity> &particles() const { return _contents; }

            template<int Dimension>
            [[nodiscard]] std::pair<std::span<NBody::Entity>, std::span<NBody::Entity>>
            split(std::span<NBody::Entity> &entities, const NBody::Simulation &registry) const {

                auto c = std::partition(entities.begin(), entities.end(), [&](Entity entity) {
                    return (registry.template get<Physics::Position>(entity))[Dimension] < _center[Dimension];
                });

                std::span less{&*entities.begin(), static_cast<std::size_t>(c - entities.begin())};
                std::span more{&*c, static_cast<std::size_t>(entities.end() - c)};
                assert(less.size() + more.size() == entities.size());
                return {less, more};
            }

            [[nodiscard]] Physics::Acceleration applyRule(const Physics::Rule &rule, const Simulation &simulation,
                                                          const Physics::Position &passivePosition,
                                                          const Physics::PassiveMass &passiveMass) const {

                // Empty nodes can be ignored
                if (_contents.empty()) return Physics::Acceleration{};

                if (!_children) {

                    return std::transform_reduce(
                            _contents.begin(), _contents.end(),
                            Physics::Acceleration{}, std::plus{},
                            [&](auto entity) {
                                return rule(simulation.get<Position>(entity),
                                            simulation.get<ActiveMass>(entity),
                                            passivePosition, passiveMass);
                            });

                } else if ((_sideLength / glm::distance((glm::vec3) passivePosition, (glm::vec3) centerOfMass())) < 1) {

                    // Node is treated as a single particle if S/D < theta (where S = sideLength and D = distance)
                    return rule(centerOfMass(),
                                totalMass(),
                                passivePosition, passiveMass);

                } else {

                    return std::transform_reduce(
                            children().begin(), children().end(),
                            Physics::Acceleration{}, std::plus{},
                            [&](const auto &child) {
                                return child.applyRule(rule, simulation, passivePosition, passiveMass);
                            });
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

        Octree(NBody::Simulation &simulation, int maxDepth = std::numeric_limits<int>::max()) : _simulation(simulation) {

            // Get a list of physics-actor particles
            auto activeParticlesView = _simulation.view<Physics::ActiveMass>();
            _particles = {activeParticlesView.begin(), activeParticlesView.end()};

            _root = OctreeNode{{_particles}, _simulation};
        }


        Physics::Acceleration applyRule(const Physics::Rule &rule,
                                        const Physics::Position &passivePosition,
                                        const Physics::PassiveMass &passiveMass,
                                        float theta) {

            return _root.applyRule(rule, _simulation, passivePosition, passiveMass);

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
                                            _simulation.get<ActiveMass>(entity),
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
