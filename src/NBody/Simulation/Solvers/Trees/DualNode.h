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

        Mass _totalMass{0.0f};
        Position _centerOfMass{0.0f, 0.0f, 0.0f};

        Acceleration _acceleration{0.0f, 0.0f, 0.0f};

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

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

        [[nodiscard]] const Acceleration &acceleration() const { return _acceleration; }

        Acceleration &acceleration() { return _acceleration; }

        template<typename ViewType>
        void summarize(const ViewType &allParticles) {

            NodeImplementation::summarize(allParticles);

            _totalMass = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};

            if (isLeaf()) {

                for (const auto &entity: contents()) {
                    if (allParticles.template all_of<Position, Mass, ActiveTag>(entity)) {
                        auto entityPosition = allParticles.template get<const Position>(entity);
                        auto entityMass = allParticles.template get<const Mass>(entity).mass();
                        _totalMass.mass() += entityMass;
                        _centerOfMass = _centerOfMass + (entityMass * entityPosition);
                    }
                }
                _centerOfMass = _centerOfMass / _totalMass.mass();

            } else {

                for (const auto &child: children()) {

                    _totalMass.mass() += child.totalMass().mass();
                    _centerOfMass = _centerOfMass +
                                    (child.centerOfMass() * child.totalMass().mass());
                }
                _centerOfMass = _centerOfMass / _totalMass.mass();

            }

            // Ensure the center of mass is inside the bounding box
            // (this might not be true because of rounding errors)
            // todo: I'd like to find a way to make this unnecessary
            _centerOfMass =
                    glm::min(glm::max((glm::vec3) boundingBox().min(), _centerOfMass), boundingBox().max());
        }

        void collapseAccelerations(const entt::basic_view<entt::entity, entt::exclude_t<>, Acceleration> &context,
                                   Acceleration netAcceleration = {0.0f, 0.0f, 0.0f}) const {

            netAcceleration += (glm::vec3) acceleration();

            if (isLeaf()) {

                // When we reach a leaf node, apply the local force to all contained points
                for (auto i: contents())
                    context.get<Acceleration>(i) += (glm::vec3) netAcceleration;

            } else {

                // Descend the tree recursively, keeping track of the net acceleration over the current region
                for (const auto &child: children())
                    child.collapseAccelerations(context, netAcceleration);

            }
        }
    };
}

#endif //N_BODY_DUALNODE_H
