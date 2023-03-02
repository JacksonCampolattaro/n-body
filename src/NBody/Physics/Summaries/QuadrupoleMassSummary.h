//
// Created by Jackson Campolattaro on 2/12/23.
//

#ifndef N_BODY_QUADRUPOLEMASSSUMMARY_H
#define N_BODY_QUADRUPOLEMASSSUMMARY_H

#include "CenterOfMassSummary.h"

#include <NBody/Physics/Multipole.h>

namespace NBody {

    using namespace Physics;

    class QuadrupoleMassSummary : protected CenterOfMassSummary {
    private:

        SymmetricTensor3<2> _moment{};

    public:

        using CenterOfMassSummary::Context;
        using CenterOfMassSummary::context;

        using CenterOfMassSummary::CenterOfMassSummary;
        using CenterOfMassSummary::totalMass;
        using CenterOfMassSummary::centerOfMass;

        template<typename C>
        void summarize(const std::span<Entity> &entities, const C &context) {
            CenterOfMassSummary::summarize(entities, context);

            _moment = std::transform_reduce(
                    entities.begin(), entities.end(),
                    SymmetricTensor3<2>{}, std::plus<>(),
                    [&](const auto &entity) {
                        auto entityMass = context.template get<const Mass>(entity).mass();
                        auto entityPosition = context.template get<const Position>(entity);
                        glm::vec3 d = entityPosition - centerOfMass();
                        return SymmetricTensor3<2>::cartesianPower(d).traceless() * entityMass * 3.0f;
                    }
            );
            _moment.enforceTraceless();

        }

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {
            CenterOfMassSummary::summarize(childNodes);

            _moment = std::transform_reduce(
                    childNodes.begin(), childNodes.end(),
                    SymmetricTensor3<2>{}, std::plus<>(),
                    [&](const auto &child) {
                        auto childMass = child.summary().totalMass().mass();
                        glm::vec3 d = child.summary().centerOfMass() - centerOfMass();
                        return SymmetricTensor3<2>::cartesianPower(d).traceless() * childMass * 3.0f +
                               child.summary().moment();
                    }
            );
            _moment.enforceTraceless();

        }

        SymmetricTensor3<2> &moment() { return _moment; }

        [[nodiscard]] const SymmetricTensor3<2> &moment() const { return _moment; }

    };
}

#endif //N_BODY_QUADRUPOLEMASSSUMMARY_H
