//
// Created by Jackson Campolattaro on 7/3/22.
//

#ifndef N_BODY_OCTREE_H
#define N_BODY_OCTREE_H

#include <NBody/Simulation/Simulation.h>

#include <span>
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

            // Recursive constructor is gross, this will be replaced
            OctreeNode(std::span<NBody::Entity> contents, const NBody::Simulation &registry,
                       Physics::Position center = {}, float sideLength = (2 << 20))
                    : _contents(contents),
                      _center(center),
                      _sideLength(sideLength) {

                // Subdivide if this node contains too many particles
                // todo We can probably get more performance by allowing leaf nodes to contain more than one particle
                if (contents.size() > 10 && sideLength > 0.0001) {

                    // todo This is an awful, ugly way of doing this; later it can be replaced with bitset indices

                    // Split on x-axis
                    auto [x0, x1] = split<0>(_contents, registry);

                    // Split on y-axis
                    auto [xy00, xy01] = split<1>(x0, registry);
                    auto [xy10, xy11] = split<1>(x1, registry);

                    // Split on z-axis
                    auto [xyz000, xyz001] = split<2>(xy00, registry);
                    auto [xyz010, xyz011] = split<2>(xy01, registry);
                    auto [xyz100, xyz101] = split<2>(xy10, registry);
                    auto [xyz110, xyz111] = split<2>(xy11, registry);

                    // Initialize 8 child nodes
                    float childSideLength = sideLength / 2.0f;
                    _children = std::unique_ptr<std::array<OctreeNode, 8>>(new std::array<OctreeNode, 8>{
                            {
                                    {xyz000, registry,
                                     center + glm::vec3{-childSideLength, -childSideLength, -childSideLength},
                                     childSideLength},
                                    {xyz001, registry,
                                     center + glm::vec3{-childSideLength, -childSideLength, childSideLength},
                                     childSideLength},
                                    {xyz010, registry,
                                     center + glm::vec3{-childSideLength, childSideLength, -childSideLength},
                                     childSideLength},
                                    {xyz011, registry,
                                     center + glm::vec3{-childSideLength, childSideLength, childSideLength},
                                     childSideLength},
                                    {xyz100, registry,
                                     center + glm::vec3{childSideLength, -childSideLength, -childSideLength},
                                     childSideLength},
                                    {xyz101, registry,
                                     center + glm::vec3{childSideLength, -childSideLength, childSideLength},
                                     childSideLength},
                                    {xyz110, registry,
                                     center + glm::vec3{childSideLength, childSideLength, -childSideLength},
                                     childSideLength},
                                    {xyz111, registry,
                                     center + glm::vec3{childSideLength, childSideLength, childSideLength},
                                     childSideLength}
                            }
                    });

                }

                // Summarize mass & position
                if (_children) {

                    for (const auto &child: *_children) {
                        _totalMass.mass() += child.totalMass().mass();
                        _centerOfMass = _centerOfMass + (child.centerOfMass() * child.totalMass().mass());
                    }
                    _centerOfMass = _centerOfMass / _totalMass.mass();

                } else {

                    for (const auto &entity: _contents) {
                        auto entityMass = registry.get<ActiveMass>(entity).mass();
                        _totalMass.mass() += entityMass;
                        _centerOfMass = _centerOfMass + (entityMass * registry.get<Position>(entity));
                    }
                    _centerOfMass = _centerOfMass / _totalMass.mass();

                }
            }

            [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

            [[nodiscard]] const ActiveMass &totalMass() const { return _totalMass; }

            template<int Dimension>
            [[nodiscard]] std::pair<std::span<NBody::Entity>, std::span<NBody::Entity>>
            split(std::span<NBody::Entity> entities, const NBody::Simulation &registry) const {

                auto c = std::partition(entities.begin(), entities.end(), [&](Entity entity) {
                    assert(registry.valid(entity));
                    return (registry.template get<Physics::Position>(entity))[Dimension] < _center[Dimension];
                });

                std::span first{&*entities.begin(), static_cast<std::size_t>(c - entities.begin())};
                std::span second{&*c, static_cast<std::size_t>(entities.end() - c)};
                return {first, second};
            }

            [[nodiscard]] std::bitset<3> getIndex(const Physics::Position &position) const {
                // todo These indices might need to be reversed
                std::bitset<3> bitset;
                bitset[0] = position.x > _center.x;
                bitset[1] = position.y > _center.y;
                bitset[2] = position.z > _center.z;
                return bitset;
            }

            [[nodiscard]] OctreeNode &operator[](const std::bitset<3> &index) {
                assert(_children);
                return _children->at(index.to_ulong());
            }

            [[nodiscard]] Physics::Acceleration applyRule(const Physics::Rule &rule, const Simulation &simulation,
                                            const Physics::Position &passivePosition,
                                            const Physics::PassiveMass &passiveMass) const {

                Physics::Acceleration acceleration;

                // Empty nodes can be ignored
                if (_contents.empty()) return {};

                if (!_children) {
                    // Base case
                    for (auto entity: _contents)
                        acceleration += rule(simulation.get<Position>(entity),
                                             simulation.get<ActiveMass>(entity),
                                             passivePosition, passiveMass);

                } else if ((_sideLength / glm::distance((glm::vec3) passivePosition, (glm::vec3) centerOfMass())) < 1) {
                    // Node is treated as a single particle if S/D < theta (where S = sideLength and D = distance)
                    acceleration += rule(centerOfMass(),
                                         totalMass(),
                                         passivePosition, passiveMass);
                } else {
                    for (const auto &child: *_children) {
                        acceleration += child.applyRule(rule, simulation, passivePosition, passiveMass);
                    }
                }

                return acceleration;
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

    public:

        Octree(NBody::Simulation &simulation) : _simulation(simulation) {

            // Get a list of physics-actor particles
            auto activeParticlesView = _simulation.view<Physics::ActiveMass>();
            std::vector<NBody::Entity> activeParticles{activeParticlesView.begin(), activeParticlesView.end()};

            OctreeNode root{{activeParticles}, _simulation};
        }


        Physics::Acceleration applyRule(const Physics::Rule &rule,
                                        const Physics::Position &passivePosition,
                                        const Physics::PassiveMass &passiveMass) {

            Physics::Acceleration acceleration;

            auto actors = _simulation.view<const Position, const ActiveMass>();
            actors.each([&](const Position &actorPosition, const ActiveMass &actorMass) {

                if (actorPosition == passivePosition) return;

                acceleration += rule(actorPosition, actorMass, passivePosition, passiveMass);
            });

            return acceleration;
        }


    };

}

#endif //N_BODY_OCTREE_H
