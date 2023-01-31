//
// Created by Jackson Campolattaro on 1/12/23.
//

#ifndef N_BODY_MASSTREE_H
#define N_BODY_MASSTREE_H

#include "Tree.h"

namespace NBody {

    template<typename NodeImplementation>
    class ActiveNode : public NodeImplementation {
    private:

        Mass _totalMass{0.0f};
        Position _centerOfMass{0.0f, 0.0f, 0.0f};

    public:

        using NodeImplementation::NodeImplementation;
        using NodeImplementation::isLeaf;
        using NodeImplementation::contents;
        using NodeImplementation::children;
        using NodeImplementation::boundingBox;

        static std::vector<Entity> relevantEntities(Simulation &simulation) {
            return {simulation.group<const Position, const Mass>().begin(),
                    simulation.group<const Position, const Mass>().end()};
        }

        static BoundingBox outerBoundingBox(Simulation &simulation) { return simulation.activeBoundingBox(); }

        static entt::basic_group<
                entt::entity, entt::exclude_t<>,
                entt::get_t<>,
                const NBody::Physics::Position,
                const NBody::Physics::Mass
        > constructionContext(Simulation &simulation) {
            return simulation.group<const Position, const Mass>();
        }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

        template<typename ViewType>
        void summarize(const ViewType &activeParticles) {

            NodeImplementation::summarize(activeParticles);

            _totalMass = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};

            if (isLeaf()) {

                for (const auto &entity: contents()) {
                    auto entityMass = activeParticles.template get<const Mass>(entity).mass();
                    _totalMass.mass() += entityMass;
                    _centerOfMass = _centerOfMass + (entityMass * activeParticles.template get<const Position>(entity));
                }
                _centerOfMass = _centerOfMass / _totalMass.mass();

            } else {

                for (const auto &child: children()) {
                    _totalMass.mass() += child.totalMass().mass();
                    _centerOfMass = _centerOfMass + (child.centerOfMass() * child.totalMass().mass());
                }
                _centerOfMass = _centerOfMass / _totalMass.mass();

            }

            // Ensure the center of mass is inside the bounding box
            // (this might not be true because of rounding errors)
            // todo: I'd like to find a way to make this unnecessary
            _centerOfMass = glm::min(glm::max((glm::vec3) boundingBox().min(), _centerOfMass), boundingBox().max());
        }

    };
}

#endif //N_BODY_MASSTREE_H
