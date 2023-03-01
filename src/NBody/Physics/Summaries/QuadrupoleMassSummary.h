//
// Created by Jackson Campolattaro on 2/12/23.
//

#ifndef N_BODY_QUADRUPOLEMASSSUMMARY_H
#define N_BODY_QUADRUPOLEMASSSUMMARY_H

#include "NBody/Physics/Quadrupole.h"
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

            _moment = {};

            for (const auto &entity: entities) {
                auto entityMass = context.template get<const Mass>(entity).mass();
                auto entityPosition = context.template get<const Position>(entity);

                glm::vec3 d = entityPosition - centerOfMass();

                // equation: 3 * outerProduct(d, d) - (I * |d|^2)
                _moment += (SymmetricTensor3<2>::cartesianPower(d) * 3.0f).traceless() * entityMass;

            }
            _moment.enforceTraceless();
        }

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {
            CenterOfMassSummary::summarize(childNodes);

            _moment = {};

            for (const auto &child: childNodes) {

                glm::vec3 d = child.summary().centerOfMass() - centerOfMass();
                _moment += (SymmetricTensor3<2>::cartesianPower(d) * 3.0f).traceless() *
                           child.summary().totalMass().mass() + child.summary().moment();
            }
            _moment.enforceTraceless();

        }

        SymmetricTensor3<2> &moment() { return _moment; }

        [[nodiscard]] const SymmetricTensor3<2> &moment() const { return _moment; }

    };
}

#endif //N_BODY_QUADRUPOLEMASSSUMMARY_H
