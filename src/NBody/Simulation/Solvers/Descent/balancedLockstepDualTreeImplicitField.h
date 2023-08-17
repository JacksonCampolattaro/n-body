//
// Created by Jackson Campolattaro on 5/5/23.
//

#ifndef N_BODY_BALANCEDLOCKSTEPDUALTREEIMPLICITFIELD_H
#define N_BODY_BALANCEDLOCKSTEPDUALTREEIMPLICITFIELD_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/lockstepDualTreeImplicitField.h>

namespace NBody::Descent {

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion, RuleType Rule = Gravity>
    inline void balancedLockstepDualTreeImplicitField(
            const ActiveNode &startingActiveNode, PassiveNode &passiveNode,
            const DescentCriterion &descentCriterion, Rule &rule,
            const ActiveView &activeContext,
            const PassiveView &passiveContext
    ) {


        std::queue<std::reference_wrapper<const ActiveNode>> activeNodesQueue{{startingActiveNode}};
        std::vector<std::reference_wrapper<const ActiveNode>> equalSizeActiveNodes{};

        while (!activeNodesQueue.empty()) {
            auto &activeNode = activeNodesQueue.front().get();

            Recommendation recommendation = descentCriterion(activeNode, passiveNode);
            activeNodesQueue.pop();

            if (recommendation != Recommendation::DescendActiveNode || activeNode.isLeaf()) {
                equalSizeActiveNodes.emplace_back(activeNode);
            } else {
                for (auto &child: activeNode.children())
                    activeNodesQueue.push(child);
            }
        }

        lockstepDualTreeImplicitField<ActiveNode>(
                equalSizeActiveNodes, passiveNode,
                descentCriterion, rule,
                activeContext, passiveContext
        );

    }

}

#endif //N_BODY_BALANCEDLOCKSTEPDUALTREEIMPLICITFIELD_H
