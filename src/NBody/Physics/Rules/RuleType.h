//
// Created by Jackson Campolattaro on 5/14/23.
//

#ifndef N_BODY_RULETYPE_H
#define N_BODY_RULETYPE_H

#include "NBody/Physics/Position.h"
#include "NBody/Physics/Acceleration.h"
#include "NBody/Physics/Force.h"
#include "NBody/Physics/Mass.h"
#include "NBody/Physics/Summaries/SummaryType.h"

namespace NBody::Physics {

    template<class Implementation>
    class RuleBase {
    public: // Particle-particle interaction

        [[nodiscard]] Acceleration operator()(const Position &activePosition,
                                              const Mass &activeMass,
                                              const Position &passivePosition) {
            return static_cast<Implementation *>(this)->particleParticle(activePosition, activeMass,
                                                                               passivePosition);
        }

    public: // Node-particle interaction

        template<typename ActiveNode>
        [[nodiscard]] Acceleration operator()(const ActiveNode &activeNode,
                                              const Position &passivePosition) {

            // If the active node has a center of mass, prefer that over the overall center
            Position activePosition;
            if constexpr (requires(const ActiveNode &n) { n.summary().centerOfMass(); })
                activePosition = activeNode.summary().centerOfMass();
            else
                activePosition = activeNode.center();

            return operator()(activePosition, activeNode.summary(), passivePosition);
        }

        template<ActiveSummaryType ActiveSummary>
        [[nodiscard]] Acceleration operator()(const Position &activePosition,
                                              const ActiveSummary &activeSummary,
                                              const Position &passivePosition) {
            return static_cast<Implementation *>(this)->nodeParticle(activePosition, activeSummary,
                                                                           passivePosition);
        }

    public: // Particle-node interaction

        template<typename PassiveNode>
        requires PassiveSummaryType<typename PassiveNode::Summary>
        void operator()(
                const Position &activePosition,
                const Mass &activeMass,
                PassiveNode &passiveNode
        ) {
            operator()(activePosition, activeMass, passiveNode.center(), passiveNode.summary());
        }

        template<PassiveSummaryType PassiveSummary>
        void operator()(
                const Position &activePosition,
                const Mass &activeMass,
                const Position &passivePosition,
                PassiveSummary &passiveSummary
        ) {
            static_cast<Implementation *>(this)->particleNode(activePosition, activeMass,
                                                                    passivePosition, passiveSummary);
        }

    public: // Node-node interaction

        template<typename ActiveNode, typename PassiveNode>
        void operator()(
                const ActiveNode &activeNode,
                PassiveNode &passiveNode
        ) {

            // Self-interaction should never happen!
            assert((void *) &activeNode != (void *) &passiveNode);

            // If the active node has a center of mass, prefer that over the overall center
            Position activePosition;
            if constexpr (requires(const ActiveNode &n) { n.summary().centerOfMass(); })
                activePosition = activeNode.summary().centerOfMass();
            else
                activePosition = activeNode.center();

            operator()(activePosition, activeNode.summary(),
                       passiveNode.center(), passiveNode.summary());
        }

        template<ActiveSummaryType ActiveSummary, PassiveSummaryType PassiveSummary>
        void operator()(
                const Position &activePosition,
                const ActiveSummary &activeSummary,
                const Position &passivePosition,
                PassiveSummary &passiveSummary
        ) {
            static_cast<Implementation *>(this)->nodeNode(activePosition, activeSummary,
                                                                passivePosition, passiveSummary);
        }

    };

    template<typename T>
    concept RuleType = std::is_base_of_v<RuleBase<T>, T>;
}

#endif //N_BODY_RULETYPE_H
