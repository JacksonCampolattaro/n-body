//
// Created by Jackson Campolattaro on 5/30/23.
//

#ifndef N_BODY_DIAGONALOVERSEPARATION_H
#define N_BODY_DIAGONALOVERSEPARATION_H

#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>

namespace NBody::Descent {

    class DiagonalOverSeparation : public ThetaDescentCriterion {
    public:

        using ThetaDescentCriterion::ThetaDescentCriterion;

        template<NodeType TreeNode>
        inline bool operator()(const TreeNode &activeNode, const Position &point) const {
            return (activeNode.boundingBox().diagonalLength() / separation(activeNode.boundingBox(), point)) < _theta
                   && activeNode.contents().size() > 32;
        }

        template<NodeType TreeNode>
        inline bool operator()(const Position &point, const TreeNode &passiveNode) const {
            return (3 * passiveNode.boundingBox().diagonalLength() / separation(passiveNode.boundingBox(), point))
                   < _theta;
        }

        template<NodeType ActiveTreeNode, NodeType PassiveTreeNode>
        inline Recommendation operator()(const ActiveTreeNode &activeNode, const PassiveTreeNode &passiveNode) const {

            float activeDiagonal = activeNode.boundingBox().diagonalLength();
            float passiveDiagonal = passiveNode.boundingBox().diagonalLength();

            if ((activeDiagonal + passiveDiagonal) / separation(activeNode.boundingBox(), passiveNode.boundingBox())
                < _theta && activeNode.contents().size() > 32)
                return Recommendation::Approximate;

            // Descend the passive node, as long as it's more than half the size of the active node
            Recommendation shouldDescendPassive = (activeDiagonal <= passiveDiagonal * 2.0f) ?
                                                  Recommendation::DescendPassiveNode : Recommendation::DoNotDescend;

            // Descend the active node, as long as it's more than half the size of the passive node
            Recommendation shouldDescendActive = (passiveDiagonal <= activeDiagonal * 2.0f) ?
                                                 Recommendation::DescendActiveNode : Recommendation::DoNotDescend;

            return shouldDescendPassive | shouldDescendActive;

        }

    };

}

#endif //N_BODY_DIAGONALOVERSEPARATION_H
