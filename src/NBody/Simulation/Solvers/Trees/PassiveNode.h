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
        using NodeImplementation::summarize;

        static std::vector<Entity> relevantEntities(Simulation &simulation) {
            // The field tree only needs to contain passive particles (which receive forces)
            return {simulation.group<const Position>(entt::get<PassiveTag>).begin(),
                    simulation.group<const Position>(entt::get<PassiveTag>).end()};
        }

        [[nodiscard]] const Acceleration &acceleration() const { return _acceleration; }

        Acceleration &acceleration() { return _acceleration; }

//        template<typename ViewType>
//        void summarize(const ViewType &activeParticles) {
//            NodeImplementation::summarize(activeParticles);
//        }

        void collapseAccelerations(const entt::basic_view<entt::entity, entt::exclude_t<>, Acceleration> &accelerations,
                                   const Acceleration &netAcceleration) const {

            Acceleration localAcceleration = netAcceleration + acceleration();

            if (isLeaf()) {

                // When we reach a leaf node, apply the local acceleration to all contained points
                for (auto i: contents())
                    accelerations.get<Acceleration>(i) += localAcceleration;

            } else {

                // Descend the tree recursively, keeping track of the net gradient over the current region
                for (const auto &child: children())
                    child.collapseAccelerations(accelerations, localAcceleration);

            }
        }
    };

    template<typename TreeNode>
    class AdvancedPassiveTreeNode : public PassiveNode<TreeNode> {
    private:

        Position _centerOfMass{0.0f, 0.0f, 0.0f};

    public:

        using PassiveNode<TreeNode>::PassiveTreeNode;
        using PassiveNode<TreeNode>::acceleration;
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
