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

        Mass _totalMass;
        Force _force{0.0f, 0.0f, 0.0f};

    public:

        using NodeImplementation::NodeImplementation;
        using NodeImplementation::isLeaf;
        using NodeImplementation::contents;
        using NodeImplementation::children;

        template<typename ViewType>
        void summarize(const ViewType &context) {

            NodeImplementation::summarize(context);

            _totalMass = 0.0f;

            if (isLeaf()) {
                for (const auto &entity: contents()) {
                    auto entityMass = context.template get<const Mass>(entity).mass();
                    _totalMass.mass() += entityMass;
                }
            } else {
                for (const auto &child: children()) {
                    _totalMass.mass() += child.totalMass().mass();
                }
            }
        }


        static std::vector<Entity> relevantEntities(Simulation &simulation) {
            // The field tree only needs to contain passive particles (which receive forces)
            return {simulation.view<const Position, const Mass, const PassiveTag>().begin(),
                    simulation.view<const Position, const Mass, const PassiveTag>().end()};
        }

        [[nodiscard]] const Force &force() const { return _force; }

        Force &force() { return _force; }

        const Mass &totalMass() const { return _totalMass; }

        Mass &totalMass() { return _totalMass; }

        void collapseForces(const entt::basic_view<entt::entity, entt::exclude_t<>, Force> &forces,
                            Force netForce = {0.0f, 0.0f, 0.0f}) const {

            netForce += (glm::vec3) force();

            if (isLeaf()) {

                // When we reach a leaf node, apply the local force to all contained points
                for (auto i: contents())
                    forces.get<Force>(i) += (glm::vec3) netForce;

            } else {

                // Descend the tree recursively, keeping track of the net gradient over the current region
                for (const auto &child: children())
                    child.collapseForces(forces, netForce);

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
