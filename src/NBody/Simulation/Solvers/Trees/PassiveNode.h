//
// Created by Jackson Campolattaro on 1/12/23.
//

#ifndef N_BODY_PASSIVENODE_H
#define N_BODY_PASSIVENODE_H

#include "Tree.h"

namespace NBody {

    template<typename NodeImplementation>
    class PassiveNode : public NodeImplementation {
    private:

        Acceleration _acceleration{0.0f, 0.0f, 0.0f};

    public:

        using NodeImplementation::NodeImplementation;
        using NodeImplementation::isLeaf;
        using NodeImplementation::contents;
        using NodeImplementation::children;

        template<typename ViewType>
        void summarize(const ViewType &context) { NodeImplementation::summarize(context); }

        static std::vector<Entity> relevantEntities(Simulation &simulation) {
            // The field tree only needs to contain passive particles (which receive gravitational acceleration)
            return {simulation.view<const Position, const Acceleration>().begin(),
                    simulation.view<const Position, const Acceleration>().end()};
        }

        static BoundingBox outerBoundingBox(Simulation &simulation) { return simulation.passiveBoundingBox(); }

        static entt::basic_view<
                entt::entity, entt::exclude_t<>,
                const NBody::Physics::Position,
                const NBody::Physics::Acceleration
        > constructionContext(Simulation &simulation) {
            return simulation.template view<const Position, const Acceleration>();
        }

        [[nodiscard]] const Acceleration &acceleration() const { return _acceleration; }

        Acceleration &acceleration() { return _acceleration; }

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

    template<typename TreeNode>
    class AdvancedPassiveTreeNode : public PassiveNode<TreeNode> {
    private:

        Position _centerOfMass{0.0f, 0.0f, 0.0f};

    public:

        using PassiveNode<TreeNode>::PassiveTreeNode;
        using PassiveNode<TreeNode>::force;
        using PassiveNode<TreeNode>::isLeaf;
        using PassiveNode<TreeNode>::contents;
        using PassiveNode<TreeNode>::children;

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

        template<typename ViewType>
        void summarize(const ViewType &passiveParticles) {

            float totalMass = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};

            for (const auto &entity: contents()) {
                auto entityMass = passiveParticles.template get<const Mass>(entity).mass();
                totalMass += entityMass;
                _centerOfMass = _centerOfMass + (entityMass * passiveParticles.template get<const Position>(entity));
            }
            _centerOfMass = _centerOfMass / totalMass;
        }
    };
}

#endif //N_BODY_PASSIVENODE_H
