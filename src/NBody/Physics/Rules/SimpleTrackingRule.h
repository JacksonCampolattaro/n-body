//
// Created by Jackson Campolattaro on 5/14/23.
//

#ifndef N_BODY_SIMPLETRACKINGRULE_H
#define N_BODY_SIMPLETRACKINGRULE_H

#include <NBody/Physics/Rules/RuleType.h>
#include <NBody/Physics/Rules/Gravity.h>

namespace NBody::Physics {

    template<RuleType Rule = Gravity>
    class SimpleTrackingRule : public RuleBase<SimpleTrackingRule<Rule>> {
    private:

        Rule _underlyingRule;

        std::size_t _particleParticleCount = 0;
        std::size_t _particleNodeCount = 0;
        std::size_t _nodeParticleCount = 0;
        std::size_t _nodeNodeCount = 0;

    public:

        template<typename ...Args>
        [[nodiscard]] Acceleration particleParticle(Args &&... args) {
            _particleParticleCount++;
            return _underlyingRule.particleParticle(std::forward<Args>(args)...);
        }

        template<typename ...Args>
        [[nodiscard]] Acceleration particleNode(Args &&... args) {
            _particleNodeCount++;
            return _underlyingRule.particleNode(std::forward<Args>(args)...);
        }

        template<typename ...Args>
        [[nodiscard]] Acceleration nodeParticle(Args &&... args) {
            _nodeParticleCount++;
            return _underlyingRule.nodeParticle(std::forward<Args>(args)...);
        }

        template<typename ...Args>
        [[nodiscard]] Acceleration nodeNode(Args &&... args) {
            _nodeNodeCount++;
            return _underlyingRule.nodeNode(std::forward<Args>(args)...);
        }


    };

}

#endif //N_BODY_SIMPLETRACKINGRULE_H
