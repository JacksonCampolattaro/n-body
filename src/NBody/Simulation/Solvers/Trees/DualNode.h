//
// Created by Jackson Campolattaro on 1/26/23.
//

#ifndef N_BODY_DUALNODE_H
#define N_BODY_DUALNODE_H

#include "Tree.h"

namespace NBody {

    template<typename NodeImplementation>
    class DualNode : public NodeImplementation {
    private:

        Mass _totalActiveMass{0.0f};
        Position _centerOfActiveMass{0.0f, 0.0f, 0.0f};

        Mass _totalPassiveMass{0.0f};
        Force _force{0.0f, 0.0f, 0.0f};

    public:

        using NodeImplementation::NodeImplementation;
        using NodeImplementation::isLeaf;
        using NodeImplementation::contents;
        using NodeImplementation::children;
        using NodeImplementation::boundingBox;

        static std::vector<Entity> relevantEntities(Simulation &simulation) {
            return {simulation.view<const Position>().begin(),
                    simulation.view<const Position>().end()};
        }

        static Simulation &constructionContext(Simulation &simulation) {
            return simulation;
        }

        [[nodiscard]] const Mass &totalActiveMass() const { return _totalActiveMass; }

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfActiveMass; }

        [[nodiscard]] const Mass &totalPassiveMass() const { return _totalActiveMass; }

        [[nodiscard]] const Force &force() const { return _force; }

        Force &force() { return _force; }

        template<typename ViewType>
        void summarize(const ViewType &allParticles) {

            NodeImplementation::summarize(allParticles);

            _totalActiveMass = 0.0f;
            _centerOfActiveMass = {0.0f, 0.0f, 0.0f};

            _totalPassiveMass = 0.0f;

            if (isLeaf()) {

                for (const auto &entity: contents()) {
                    if (allParticles.template all_of<Position, Mass, ActiveTag>(entity)) {
                        auto entityPosition = allParticles.template get<const Position>(entity);
                        auto entityMass = allParticles.template get<const Mass>(entity).mass();
                        _totalActiveMass.mass() += entityMass;
                        _centerOfActiveMass = _centerOfActiveMass + (entityMass * entityPosition);
                    }
                    if (allParticles.template all_of<Position, Mass, PassiveTag>(entity)) {
                        auto entityPosition = allParticles.template get<const Position>(entity);
                        auto entityMass = allParticles.template get<const Mass>(entity).mass();
                        _totalPassiveMass.mass() += entityMass;
                    }
                }
                _centerOfActiveMass = _centerOfActiveMass / _totalActiveMass.mass();

            } else {

                for (const auto &child: children()) {

                    _totalActiveMass.mass() += child.totalActiveMass().mass();
                    _centerOfActiveMass = _centerOfActiveMass +
                                          (child.centerOfMass() * child.totalActiveMass().mass());
                    _totalPassiveMass.mass() += child.totalPassiveMass().mass();
                }
                _centerOfActiveMass = _centerOfActiveMass / _totalActiveMass.mass();

            }

            // Ensure the center of mass is inside the bounding box
            // (this might not be true because of rounding errors)
            // todo: I'd like to find a way to make this unnecessary
            _centerOfActiveMass =
                    glm::min(glm::max((glm::vec3) boundingBox().min(), _centerOfActiveMass), boundingBox().max());
        }

        void collapseForces(const entt::basic_view<entt::entity, entt::exclude_t<>, const Mass, Force> &context,
                            Force netForce = {0.0f, 0.0f, 0.0f}) const {

            netForce += (glm::vec3) force();

            if (isLeaf()) {

                // When we reach a leaf node, apply the local force to all contained points
                // todo: the force should be applied proportionally by mass
                for (auto i: contents())
                    context.get<Force>(i) += (glm::vec3) netForce
                                             * (context.get<const Mass>(i).mass() / totalPassiveMass().mass());

            } else {

                // Descend the tree recursively, keeping track of the net force over the current region
                for (const auto &child: children())
                    child.collapseForces(
                            context,
                            netForce * (child.totalPassiveMass().mass() / totalPassiveMass().mass())
                    );

            }
        }
    };
}

#endif //N_BODY_DUALNODE_H
