//
// Created by Jackson Campolattaro on 5/14/23.
//

#ifndef N_BODY_SIMPLETRACKINGRULE_H
#define N_BODY_SIMPLETRACKINGRULE_H

#include <NBody/Physics/Rules/RuleType.h>
#include <NBody/Physics/Rules/Gravity.h>

namespace NBody::Physics {

    template<RuleType Rule>
    class SimpleTrackingRule : public RuleBase<SimpleTrackingRule<Rule>> {
    private:

        Rule _underlyingRule;

        std::atomic<int> _particleParticleCount = 0;
        std::atomic<int> _particleNodeCount = 0;
        std::atomic<int> _nodeParticleCount = 0;
        std::atomic<int> _nodeNodeCount = 0;

    public:

        template<typename ...Args>
        SimpleTrackingRule(Args &&... args) : _underlyingRule(std::forward<Args>(args)...) {}

        template<typename ...Args>
        [[nodiscard]] Acceleration particleParticle(Args &&... args) {
            _particleParticleCount.fetch_add(1, std::memory_order_relaxed);
            return {}; //_underlyingRule.particleParticle(std::forward<Args>(args)...);
        }

        template<typename ...Args>
        void particleNode(Args &&... args) {
            _particleNodeCount.fetch_add(1, std::memory_order_relaxed);
            //_underlyingRule.particleNode(std::forward<Args>(args)...);
        }

        template<typename ...Args>
        [[nodiscard]] Acceleration nodeParticle(Args &&... args) {
            _nodeParticleCount.fetch_add(1, std::memory_order_relaxed);
            return {};//_underlyingRule.nodeParticle(std::forward<Args>(args)...);
        }

        template<typename ...Args>
        void nodeNode(Args &&... args) {
            _nodeNodeCount.fetch_add(1, std::memory_order_relaxed);
            //_underlyingRule.nodeNode(std::forward<Args>(args)...);
        }

    public:

        const Rule &underlyingRule() { return _underlyingRule; }

        [[nodiscard]] std::size_t particleParticleCount() const { return _particleParticleCount; }

        [[nodiscard]] std::size_t particleNodeCount() const { return _particleNodeCount; }

        [[nodiscard]] std::size_t nodeParticleCount() const { return _nodeParticleCount; }

        [[nodiscard]] std::size_t nodeNodeCount() const { return _nodeNodeCount; }

        [[nodiscard]] std::size_t totalCount() const {
            return _particleParticleCount + _particleNodeCount + _nodeParticleCount + _nodeNodeCount;
        }

        [[nodiscard]] std::string toString() const {
            return fmt::format(
                    "pp:{}, pn:{}, np:{}, nn:{}",
                    particleParticleCount(),
                    particleNodeCount(),
                    nodeParticleCount(),
                    nodeNodeCount()
            );
        }

    public:

        friend std::ostream &operator<<(std::ostream &o, const SimpleTrackingRule<Rule> &rule) {
            return o << rule.toString();
        }

    };

}

#endif //N_BODY_SIMPLETRACKINGRULE_H
