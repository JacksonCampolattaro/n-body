//
// Created by Jackson Campolattaro on 1/10/23.
//

#ifndef N_BODY_DESCENTCRITERIONTYPE_H
#define N_BODY_DESCENTCRITERIONTYPE_H

#include <glm/glm.hpp>

#include <NBody/Physics/Position.h>

#include <NBody/Simulation/Solvers/Trees/Octree.h>

namespace NBody::Descent {

    // todo: a c++20 "DescentCriterion" concept would be appropriate here

    using Physics::Position;

    enum class Recommendation {
        DoNotDescend = 0,
        Approximate = 0,
        DescendActiveNode = 1 << 0,
        DescendPassiveNode = 1 << 1,
        DescendBothNodes = (DescendActiveNode | DescendPassiveNode)
    };

    inline Recommendation operator|(Recommendation a, Recommendation b) {
        return static_cast<Recommendation>(static_cast<int>(a) | static_cast<int>(b));
    }

    template<typename T>
    concept DescentCriterionType = requires(T &t, const typename ActiveOctree::Node &n, const Position &p) {
        { t(n, p) } -> std::convertible_to<bool>;
    } && requires(T &t, const typename ActiveOctree::Node &an, const typename PassiveOctree::Node &pn) {
        { t(an, pn) } -> std::convertible_to<Recommendation>;
    };

    class ThetaDescentCriterion {
    protected:

        float _theta;

    public:

        explicit ThetaDescentCriterion(float theta = 0.5f) : _theta(theta) {}

        [[nodiscard]] const float &theta() const { return _theta; }

        float &theta() { return _theta; }
    };

}

#endif //N_BODY_DESCENTCRITERIONTYPE_H
