//
// Created by Jackson Campolattaro on 6/8/23.
//

#ifndef N_BODY_ACCURACYTRACKINGRULE_H
#define N_BODY_ACCURACYTRACKINGRULE_H

#include <NBody/Physics/Rules/RuleType.h>
#include <NBody/Physics/Rules/Gravity.h>

namespace NBody::Physics {

    template<RuleType Rule = Gravity>
    class AccuracyTrackingRule : public RuleBase<AccuracyTrackingRule<Rule>> {
    private:

        Rule _underlyingRule;
        const Simulation &_simulation;

        std::ofstream _output;
        std::mutex _mutex;

    public:

        template<typename ...Args>
        explicit AccuracyTrackingRule(const std::string &path, const Simulation &simulation, Args &&... args) :
                _output(path),
                _simulation(simulation),
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
                       "Passive is Leaf,"
                       "Absolute Error,"
                       "Relative Error"
                       "\n" << std::fixed;
        }

        template<typename ...Args>
        [[nodiscard]] Acceleration particleParticle(Args &&... args) {
            std::scoped_lock l{_mutex};
            return _underlyingRule.particleParticle(std::forward<Args>(args)...);
        }

        template<typename ActiveNode>
        [[nodiscard]] Acceleration nodeParticle(const ActiveNode &activeNode,
                                                const Position &passivePosition) {

            Acceleration actualAcceleration = std::transform_reduce(
                    activeNode.contents().begin(), activeNode.contents().end(),
                    Physics::Acceleration{}, std::plus{},
                    [&](auto &activeEntity) {
                        return _underlyingRule(_simulation.get<const Position>(activeEntity),
                                               _simulation.get<const Mass>(activeEntity),
                                               passivePosition);
                    }
            );
            Acceleration approximateAcceleration = _underlyingRule.nodeParticle(
                    activeNode.summary().centerOfMass(),
                    activeNode.summary(),
                    passivePosition
            );

            float error = glm::distance((glm::vec3) actualAcceleration, approximateAcceleration);
            float relativeError = error / glm::length((glm::vec3) actualAcceleration);
            float distance = glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passivePosition);

            if (relativeError > 0.005) {
                std::cout << relativeError << "\n";
                for (auto e: activeNode.contents())
                    std::cout << _simulation.get<const Position>(e) << ", "
                              << _simulation.get<const Mass>(e).mass() << "\n";
                std::cout << activeNode.boundingBox() << "\n";
                std::cout << activeNode.summary().centerOfMass() << ", "
                          << activeNode.summary().totalMass().mass() << "\n";
                std::cout << passivePosition << "\n\n\n";
            }

            std::scoped_lock l{_mutex};
            _output << "Node-Particle,"
                    << activeNode.contents().size() << ","
                    << "1,"
                    << activeNode.boundingBox().maxSideLength() << ","
                    << "0.0,"
                    << distance << ","
                    << activeNode.isLeaf() << ","
                    << ","
                    << error << ","
                    << relativeError
                    << "\n";
            return approximateAcceleration;
        }

        template<class ActiveSummaryType>
        [[nodiscard]] Acceleration nodeParticle(const Position &activePosition,
                                                const ActiveSummaryType &activeSummary,
                                                const Position &passivePosition) const {
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

            PassiveNode proxy = passiveNode;
            proxy.summary().acceleration() = {};
            _underlyingRule.particleNode(activePosition, activeMass, proxy.center(), proxy.summary());
            auto approximateAcceleration = proxy.summary().acceleration();

            float maxError = 0;
            float maxRelativeError = 0;
            for (auto &passiveEntity: passiveNode.contents()) {
                auto &passivePosition = _simulation.get<const Position>(passiveEntity);
                Acceleration localApproximateAcceleration = approximateAcceleration.at(
                        proxy.center() - passivePosition
                );
                Acceleration actualAcceleration = _underlyingRule.particleParticle(
                        activePosition, activeMass, passivePosition
                );
                maxError = std::max(
                        maxError,
                        glm::distance((glm::vec3) actualAcceleration, localApproximateAcceleration)
                );
                maxRelativeError = std::max(
                        maxRelativeError,
                        glm::distance((glm::vec3) actualAcceleration, localApproximateAcceleration) /
                        glm::length((glm::vec3) actualAcceleration)
                );
            }

            float distance = glm::distance((glm::vec3) passiveNode.center(), activePosition);

            std::scoped_lock l{_mutex};
            _output << "Particle-Node,"
                    << "1,"
                    << passiveNode.contents().size() << ","
                    << "0.0,"
                    << passiveNode.boundingBox().maxSideLength() << ","
                    << ","
                    << distance << ","
                    << ","
                    << passiveNode.isLeaf() << ","
                    << maxError << ","
                    << maxRelativeError
                    << "\n";
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

            PassiveNode proxy = passiveNode;
            proxy.summary().acceleration() = {};
            _underlyingRule.nodeNode(activeNode, proxy);
            auto approximateAcceleration = proxy.summary().acceleration();

            float maxError = 0;
            float maxRelativeError = 0;
            for (auto &passiveEntity: passiveNode.contents()) {
                auto &passivePosition = _simulation.get<const Position>(passiveEntity);
                Acceleration localApproximateAcceleration = approximateAcceleration.at(
                        proxy.center() - passivePosition
                );
                Acceleration actualAcceleration = std::transform_reduce(
                        activeNode.contents().begin(), activeNode.contents().end(),
                        Physics::Acceleration{}, std::plus{},
                        [&](auto &activeEntity) {
                            return _underlyingRule(_simulation.get<const Position>(activeEntity),
                                                   _simulation.get<const Mass>(activeEntity),
                                                   passivePosition);
                        }
                );
                maxError = std::max(
                        maxError,
                        glm::distance((glm::vec3) actualAcceleration, localApproximateAcceleration)
                );
                maxRelativeError = std::max(
                        maxRelativeError,
                        glm::distance((glm::vec3) actualAcceleration, localApproximateAcceleration) /
                        glm::length((glm::vec3) actualAcceleration)
                );
            }

            float distance = glm::distance((glm::vec3) activeNode.summary().centerOfMass(), passiveNode.center());

            std::scoped_lock l{_mutex};
            _output << "Node-Node,"
                    << activeNode.contents().size() << ","
                    << passiveNode.contents().size() << ","
                    << activeNode.boundingBox().maxSideLength() << ","
                    << passiveNode.boundingBox().maxSideLength() << ","
                    << distance << ","
                    << activeNode.isLeaf() << ","
                    << passiveNode.isLeaf() << ","
                    << maxError << ","
                    << maxRelativeError
                    << "\n";
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

#endif //N_BODY_ACCURACYTRACKINGRULE_H
