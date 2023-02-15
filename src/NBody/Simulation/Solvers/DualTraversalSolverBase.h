//
// Created by Jackson Campolattaro on 2/10/23.
//

#ifndef N_BODY_DUALTRAVERSALSOLVERBASE_H
#define N_BODY_DUALTRAVERSALSOLVERBASE_H

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Solvers/Trees/Tree.h>
#include <NBody/Simulation/Solvers/Trees/DescentCriterion.h>

namespace NBody {

    using namespace Physics;

    template<typename ActiveTree, typename PassiveTree, typename DescentCriterionType>
    class DualTraversalSolverBase : public Solver {
    protected:

        DescentCriterionType _descentCriterion{0.4f};

    public:

        using Solver::Solver;

        float &theta() { return _descentCriterion.theta(); }

        const float &theta() const { return _descentCriterion.theta(); }

    protected:

        void computeAccelerations(
                const entt::basic_view<
                        entt::entity, entt::exclude_t<>,
                        const Position, Acceleration
                > &passiveParticles,
                const entt::basic_view<
                        entt::entity, entt::exclude_t<>,
                        const Position, const Mass
                > &activeParticles,
                const typename ActiveTree::Node &activeNode,
                typename PassiveTree::Node &passiveNode) {

            // If either node is empty, we have no need to calculate forces between them
            if (activeNode.contents().empty() || passiveNode.contents().empty())
                return;

            // If the nodes are far enough apart or both leaves, we can use their summaries
            if (_descentCriterion(activeNode, passiveNode)) {

                assert(!doIntersect(activeNode.boundingBox(), passiveNode.boundingBox()));

                // node-node interaction
                passiveNode.summary().acceleration() += (glm::vec3) _rule(activeNode, passiveNode);

            } else if (activeNode.isLeaf() && passiveNode.isLeaf()) {

                // If both nodes are leaves & weren't far enough to summarize, then compute individual interactions
                // todo: maybe we can do node-particle interactions in some cases
                for (auto activeParticle: activeNode.contents()) {
                    for (auto passiveParticle: passiveNode.contents()) {

                        passiveParticles.get<Acceleration>(passiveParticle) +=
                                (glm::vec3) _rule(activeParticles.get<const Position>(activeParticle),
                                                  activeParticles.get<const Mass>(activeParticle),
                                                  passiveParticles.get<const Position>(passiveParticle));

                    }
                }

            } else {

                // If the passive node isn't a leaf, we'll descend all its children
                std::span<typename PassiveTree::Node> passiveNodesToDescend =
                        passiveNode.isLeaf() ? std::span<typename PassiveTree::Node>{&passiveNode, 1}
                                             : passiveNode.children();

                // If the active node isn't a leaf, we'll descend all its children
                std::span<const typename ActiveTree::Node> activeNodesToDescend =
                        activeNode.isLeaf() ? std::span<const typename ActiveTree::Node>{&activeNode, 1}
                                            : activeNode.children();

                // Treat every combination of force & field node
                for (auto &childPassiveNode: passiveNodesToDescend) {
                    for (const auto &childActiveNode: activeNodesToDescend) {
                        computeAccelerations(passiveParticles, activeParticles,
                                             childActiveNode, childPassiveNode);
                    }
                }
            }
        }

        void collapseAccelerations(typename PassiveTree::Node &node,
                                   const entt::basic_view<entt::entity, entt::exclude_t<>, Acceleration> &context,
                                   Acceleration netAcceleration = {0.0f, 0.0f, 0.0f}) const {

            netAcceleration += (glm::vec3) node.summary().acceleration();

            if (node.isLeaf()) {

                // When we reach a leaf node, apply the local force to all contained points
                for (auto i: node.contents())
                    context.get<Acceleration>(i) += (glm::vec3) netAcceleration;

            } else {

                // Descend the tree recursively, keeping track of the net acceleration over the current region
                for (auto &child: node.children())
                    collapseAccelerations(child, context, netAcceleration);

            }
        }

    };

}

#endif //N_BODY_DUALTRAVERSALSOLVERBASE_H
