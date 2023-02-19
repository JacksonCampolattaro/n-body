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
            if (_descentCriterion(activeNode, passiveNode) &&
                !doIntersect(activeNode.boundingBox(), passiveNode.boundingBox())) {

                // node-node interaction
                passiveNode.summary().acceleration() += _rule(activeNode, passiveNode);

            } else if (activeNode.isLeaf() && passiveNode.isLeaf()) {

                // If both nodes are leaves & weren't far enough to summarize, then compute individual interactions
                // todo: maybe we can do node-particle interactions in some cases
                for (auto activeParticle: activeNode.contents()) {
                    for (auto passiveParticle: passiveNode.contents()) {

                        passiveParticles.get<Acceleration>(passiveParticle) +=
                                _rule(activeParticles.get<const Position>(activeParticle),
                                      activeParticles.get<const Mass>(activeParticle),
                                      passiveParticles.get<const Position>(passiveParticle));

                    }
                }

            } else if (passiveNode.isLeaf()) {

                for (auto passiveParticle: passiveNode.contents()) {

                    passiveParticles.get<Acceleration>(passiveParticle) +=
                            computeAcceleration(
                                    activeParticles,
                                    activeNode,
                                    passiveParticles.get<const Position>(passiveParticle)
                            );

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

        // todo: This is duplicated from ActiveTreeSolver, and that shouldn't be necessary
        inline Acceleration computeAcceleration(
                const entt::basic_view<
                        entt::entity, entt::exclude_t<>,
                        const Position, const Mass
                > &activeParticles,
                const typename ActiveTree::Node &node,
                const Position &passivePosition
        ) {

            // Empty nodes can be ignored
            if (node.contents().empty()) return Physics::Acceleration{};

            if (_descentCriterion(node, passivePosition)) {
                return _rule(node, passivePosition);
            } else {

                // Otherwise, the node can't be summarized
                if (node.isLeaf()) {

                    // If this is a leaf node, interact with all particles contained
                    return std::transform_reduce(
                            node.contents().begin(), node.contents().end(),
                            Physics::Acceleration{}, std::plus{},
                            [&](auto entity) {
                                return _rule(activeParticles.get<const Position>(entity),
                                             activeParticles.get<const Mass>(entity),
                                             passivePosition);
                            }
                    );

                } else {

                    // If it's a non-leaf node, descend the tree (recursive case)
                    return std::transform_reduce(
                            node.children().begin(), node.children().end(),
                            Physics::Acceleration{}, std::plus{},
                            [&](const auto &child) {
                                return computeAcceleration(
                                        activeParticles,
                                        child,
                                        passivePosition
                                );
                            }
                    );
                }
            }
        }

        void collapseAccelerations(typename PassiveTree::Node &node,
                                   const entt::basic_view<
                                           entt::entity, entt::exclude_t<>,
                                           const Position,
                                           Acceleration
                                   > &context,
                                   Acceleration netAcceleration = {}) const {

            netAcceleration += node.summary().acceleration();

            if (node.isLeaf()) {

                // When we reach a leaf node, apply the local force to all contained points
                for (auto i: node.contents())
                    context.get<Acceleration>(i) += netAcceleration;

            } else {

                // Descend the tree recursively, keeping track of the net acceleration over the current region
                for (auto &child: node.children())
                    collapseAccelerations(child, context, netAcceleration);

            }
        }

        void collapseAccelerations(typename PassiveTree::Node &node,
                                   const entt::basic_view<
                                           entt::entity, entt::exclude_t<>,
                                           const Position,
                                           Acceleration
                                   > &context,
                                   QuadrupoleAcceleration netAcceleration = {}) const {

            // Add the local multipole acceleration of this node to the accumulated acceleration in this location
            netAcceleration += node.summary().acceleration();

            if (node.isLeaf()) {

                // When we reach a leaf node, apply the local force to all contained points
                for (auto i: node.contents()) {

                    // Find the vector from the center of this node to the particle's position
                    // and re-center the multipole expansion on the particle
                    //auto localAcceleration = netAcceleration.at(context.get<const Position>(i) - node.center());
                    auto localAcceleration = netAcceleration.at(node.center() - context.get<const Position>(i));

                    // Apply the xyz component of the multipole acceleration to the particle
                    context.get<Acceleration>(i) += localAcceleration;
                }

            } else {

                // Descend the tree recursively, keeping track of the net acceleration over the current region
                for (auto &child: node.children()) {

                    // Find the vector from the center of this node to the center of the child
                    // and re-center the multipole expansion on the child node's center
                    //auto localAcceleration = netAcceleration.translated(child.center() - node.center());
                    auto localAcceleration = netAcceleration.translated(node.center() - child.center());

                    // Apply the shifted expansion to the child node (recursive)
                    collapseAccelerations(child, context, localAcceleration);
                }

            }
        }

    };

}


//void fmm::fmm_force_passdown(gravnode *node_no, taylor_data taylor_current) {
//
//    // Add the external field expansion to the local one accumulated for this node
//    taylor_current.coeff.dphi += TaylorCoeff[no].coeff.dphi;
//    taylor_current.coeff.d2phi += TaylorCoeff[no].coeff.d2phi;
//
//    if (node_no.isLeaf()) {
//
//        for (particle p: node_no.contents()) {
//
//            // Find the vector from the center of this node to the particle's position
//            vector <MyReal> dxyz;
//            Tp->nearest_image_intpos_to_pos(p.position, node_no->s.da, dxyz.da);
//
//            // Re-center the multipole expansion on the particle
//            vector <MyReal> d2phidxyz = taylor_current.coeff.d2phi * dxyz;
//            vector <MyReal> dphi = taylor_current.coeff.dphi;
//            dphi += d2phidxyz;
//
//            // Apply the xyz component of the multipole acceleration to the particle
//            Tp->P[mp].GravAccel -= dphi;
//
//        }
//
//    } else {
//
//        while (gravnode node_p : node_no.children()) {
//
//            // Find the vector from the center of this node to the center of the child
//            vector <MyReal> dxyz;
//            Tp->nearest_image_intpos_to_pos(node_p->s.da, node_no->s.da, dxyz.da);
//
//            // re-center the multipole expansion on the child node's center
//            vector <MyReal> delta_dphi = taylor_current.coeff.d2phi * dxyz;
//            taylor_data taylor_sub = taylor_current;
//            taylor_sub.coeff.dphi += delta_dphi;
//
//            // Apply the shifted expansion to the child node (recursive)
//            fmm_force_passdown(node_p, taylor_sub);
//        }
//    }
//
//}

#endif //N_BODY_DUALTRAVERSALSOLVERBASE_H
