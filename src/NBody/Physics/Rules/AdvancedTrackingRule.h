//
// Created by Jackson Campolattaro on 5/20/23.
//

#ifndef N_BODY_ADVANCEDTRACKINGRULE_H
#define N_BODY_ADVANCEDTRACKINGRULE_H

#include <NBody/Physics/Rules/RuleType.h>
#include <NBody/Physics/Rules/Gravity.h>

#include <NBody/Simulation/Solvers/Trees/Octree.h>
#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>

namespace NBody::Physics {

    template<typename S>
    float representativeSize(const OctreeNode<S> &node) {
        return node.sideLength();
    }

    template<typename S>
    float representativeSize(const LinearBVHNode<S> &node) {
        return node.boundingBox().diagonalLength();
    }

    template<RuleType Rule = Gravity>
    class AdvancedTrackingRule : public RuleBase<AdvancedTrackingRule<Rule>> {
    private:

        Rule _underlyingRule;
        float _sampleRate;

        std::ofstream _output;
        std::mutex _mutex;

    public:

        template<typename ...Args>
        explicit AdvancedTrackingRule(const std::string &path, float sampleRate, Args &&... args) :
                _output(path),
                _sampleRate(sampleRate),
                _underlyingRule(std::forward<Args>(args)...) {
            std::scoped_lock l{_mutex};

            // todo: write header
            _output << "Type,"
                       "Active Count,"
                       "Passive Count,"
                       "Active Size,"
                       "Passive Size,"
                       "Distance,"
                       "Active is Leaf,"
                       "Passive is Leaf"
                       "\n";
        }

        template<typename ...Args>
        [[nodiscard]] Acceleration particleParticle(Args &&... args) {
            std::scoped_lock l{_mutex};
            //            _output << "Particle-Particle,"
            //                    << "1,"
            //                    << "1,"
            //                    << "0.0,"
            //                    << "0.0,"
            //                    << ","
            //                    << ""
            //                    << "\n";
            return _underlyingRule.particleParticle(std::forward<Args>(args)...);
        }

        template<typename ActiveNode>
        [[nodiscard]] Acceleration nodeParticle(const ActiveNode &activeNode,
                                                const Position &passivePosition) {
            if ((double)rand() < ((double)_sampleRate * RAND_MAX) && activeNode.contents().size() > 1) {
                std::scoped_lock l{_mutex};
                _output << "Node-Particle,"
                        << activeNode.contents().size() << ","
                        << "1,"
                        << representativeSize(activeNode) << ","
                        << "0.0,"
                        << glm::distance((glm::vec3)activeNode.summary().centerOfMass(), passivePosition) << ","
                        << activeNode.isLeaf() << ","
                        << ""
                        << "\n";
            }
            return _underlyingRule.nodeParticle(activeNode.summary().centerOfMass(), activeNode.summary(),
                                                passivePosition);
        }

        template<class ActiveSummaryType>
        [[nodiscard]] Acceleration nodeParticle(const Position &activePosition,
                                                const ActiveSummaryType &activeSummary,
                                                const Position &passivePosition) const {
            // todo: untracked
            return _underlyingRule.nodeParticle(activePosition, activeSummary, passivePosition);
        }

        // particle-node
        template<typename PassiveNode>
        requires PassiveSummaryType<typename PassiveNode::Summary>
        void particleNode(
                const Position &activePosition,
                const Mass &activeMass,
                PassiveNode &passiveNode
        ) {
            if ((double)rand() < ((double)_sampleRate * RAND_MAX)) {
                std::scoped_lock l{_mutex};
                _output << "Particle-Node,"
                        << "1,"
                        << passiveNode.contents().size() << ","
                        << "0.0,"
                        << representativeSize(passiveNode) << ","
                        << glm::distance((glm::vec3) passiveNode.center(), activePosition) << ","
                        << ","
                        << passiveNode.isLeaf()
                        << "\n";
            }
            _underlyingRule.particleNode(activePosition, activeMass, passiveNode.center(), passiveNode.summary());
        }

        template<PassiveSummaryType PassiveSummary>
        void particleNode(
                const Position &activePosition,
                const Mass &activeMass,
                const Position &passivePosition,
                PassiveSummary &passiveSummary
        ) {
            _underlyingRule.particleNode(activePosition, activeMass, passivePosition, passiveSummary);
        }

        template<typename ActiveNode, typename PassiveNode>
        void nodeNode(
                const ActiveNode &activeNode,
                PassiveNode &passiveNode
        ) {
            if ((double)rand() < ((double)_sampleRate * RAND_MAX) && activeNode.contents().size() > 1) {
                std::scoped_lock l{_mutex};
                _output << "Node-Node,"
                        << activeNode.contents().size() << ","
                        << passiveNode.contents().size() << ","
                        << activeNode.boundingBox().diagonalLength() << ","
                        << passiveNode.boundingBox().diagonalLength() << ","
                        << glm::distance((glm::vec3) passiveNode.center(), activeNode.summary().centerOfMass()) << ","
                        << activeNode.isLeaf() << ","
                        << passiveNode.isLeaf()
                        << "\n";
            }
            _underlyingRule.nodeNode(activeNode.summary().centerOfMass(), activeNode.summary(),
                                     passiveNode.center(), passiveNode.summary());
        }

        template<ActiveSummaryType ActiveSummary, PassiveSummaryType PassiveSummary>
        void nodeNode(
                const Position &activePosition,
                const ActiveSummary &activeSummary,
                const Position &passivePosition,
                PassiveSummary &passiveSummary
        ) {
            // todo: untracked
            _underlyingRule.nodeNode(activePosition, activeSummary, passivePosition, passiveSummary);
        }

    };

}

#endif //N_BODY_ADVANCEDTRACKINGRULE_H
