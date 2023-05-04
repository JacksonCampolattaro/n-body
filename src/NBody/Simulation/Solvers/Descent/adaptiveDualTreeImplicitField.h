//
// Created by Jackson Campolattaro on 5/3/23.
//

#ifndef N_BODY_ADAPTIVEDUALTREEIMPLICITFIELD_H
#define N_BODY_ADAPTIVEDUALTREEIMPLICITFIELD_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/activeTree.h>
#include <NBody/Simulation/Solvers/Descent/passiveTree.h>
#include <NBody/Simulation/Solvers/Descent/none.h>

#include <NBody/Physics/Summaries/EmptySummary.h>

#include <queue>

namespace NBody::Descent {

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion>
    inline void adaptiveDualTreeImplicitField(
            const std::vector<std::reference_wrapper<const ActiveNode>> &activeNodes,
            PassiveNode &passiveNode,
            const DescentCriterion &descentCriterion, const Physics::Rule &rule,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, const Mass
            > &activeContext,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, Acceleration
            > &passiveContext,
            typename PassiveNode::Summary::ImpliedSummary localField = {}
    ) {

        // Empty nodes can be ignored
        if (passiveNode.contents().empty()) return;

        // Begin by sorting the active nodes based on their recommendation
        std::vector<std::reference_wrapper<const ActiveNode>> farActiveNodes; // todo: use a span for this!
        std::vector<std::reference_wrapper<const ActiveNode>> closeActiveNodes;
        for (auto &activeNode: activeNodes) {
            auto recommendation = descentCriterion(activeNode.get(), passiveNode);

            if (recommendation == Recommendation::Approximate)
                farActiveNodes.emplace_back(activeNode);
            else if (recommendation == Recommendation::DescendPassiveNode || activeNode.get().isLeaf())
                closeActiveNodes.emplace_back(activeNode);
            else
                closeActiveNodes.insert(
                        closeActiveNodes.end(),
                        activeNode.get().children().begin(), activeNode.get().children().end()
                );
        }

        // Far-field interactions are calculated first, and contribute to the local field
        for (auto &activeNode: farActiveNodes)
            rule(
                    activeNode.get().summary().centerOfMass(),
                    activeNode.get().summary(),
                    passiveNode.center(),
                    localField
            );

        // If this is a leaf node
        if (passiveNode.isLeaf()) {

            // Apply forces for each contained particle
            for (auto passiveEntity: passiveNode.contents()) {

                // Switch to active-tree
                for (auto &activeNode: closeActiveNodes) {

                    passiveContext.get<Acceleration>(passiveEntity) += activeTree(
                            activeNode.get(),
                            passiveContext.get<const Position>(passiveEntity),
                            descentCriterion, rule,
                            activeContext
                    );

                }

                // Include the net local force from far particles
                passiveContext.get<Acceleration>(passiveEntity) += localField.acceleration().at(
                        passiveContext.get<const Position>(passiveEntity) - passiveNode.center()
                );
            }

        } else {

            // If it's a non-leaf node, descend the tree (recursive case)
            // The local field -- including forces from far entities -- is passed down
            for (auto &child: passiveNode.children())
                adaptiveDualTreeImplicitField(
                        closeActiveNodes, child,
                        descentCriterion, rule,
                        activeContext, passiveContext,
                        {localField.acceleration().translated(child.center() - passiveNode.center())}
                );
        }

    }

}

#endif //N_BODY_ADAPTIVEDUALTREEIMPLICITFIELD_H
