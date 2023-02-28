//
// Created by Jackson Campolattaro on 2/12/23.
//

#ifndef N_BODY_QUADRUPOLEMASSSUMMARY_H
#define N_BODY_QUADRUPOLEMASSSUMMARY_H

#include "NBody/Physics/Quadrupole.h"
#include "CenterOfMassSummary.h"

namespace NBody {

    using namespace Physics;

    class QuadrupoleMassSummary : protected CenterOfMassSummary {
    private:

        Quadrupole _quadrupoleMoment{};

    public:

        using CenterOfMassSummary::Context;
        using CenterOfMassSummary::context;

        using CenterOfMassSummary::CenterOfMassSummary;
        using CenterOfMassSummary::totalMass;
        using CenterOfMassSummary::centerOfMass;

        template<typename C>
        void summarize(const std::span<Entity> &entities, const C &context) {
            CenterOfMassSummary::summarize(entities, context);

            _quadrupoleMoment = {};

            for (const auto &entity: entities) {
                auto entityMass = context.template get<const Mass>(entity).mass();
                auto entityPosition = context.template get<const Position>(entity);

                glm::vec3 d = entityPosition - centerOfMass();

                // equation: 3 * outerProduct(d, d) - (I * |d|^2)

                // 3.0 * d^Order
                glm::mat3 m = glm::outerProduct(d, d) * 3.0f;

                // ... made traceless
                float trace = m[0][0] + m[1][1] + m[2][2];
                m[0][0] -= trace / 3.0f;
                m[1][1] -= trace / 3.0f;
                m[2][2] -= trace / 3.0f; // this is optimized out automatically!

                // ... and multiplied by the mass
                _quadrupoleMoment += Quadrupole{m} * entityMass;

            }
            // zz only needs to be computed once
            _quadrupoleMoment.zz() = -_quadrupoleMoment.xx() - _quadrupoleMoment.yy();
        }

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {
            CenterOfMassSummary::summarize(childNodes);

            _quadrupoleMoment = {};

            for (const auto &child: childNodes) {

                float qx = child.summary().centerOfMass().x - centerOfMass().x;
                float qy = child.summary().centerOfMass().y - centerOfMass().y;
                float qz = child.summary().centerOfMass().z - centerOfMass().z;
                float qr2 = qx * qx + qy * qy + qz * qz;

                _quadrupoleMoment.xx() += child.summary().moment().xx() +
                                          child.summary().totalMass().mass() * (3.0f * qx * qx - qr2);
                _quadrupoleMoment.xy() += child.summary().moment().xy() +
                                          child.summary().totalMass().mass() * (3.0f * qx * qy);
                _quadrupoleMoment.xz() += child.summary().moment().xz() +
                                          child.summary().totalMass().mass() * (3.0f * qx * qz);
                _quadrupoleMoment.yy() += child.summary().moment().yy() +
                                          child.summary().totalMass().mass() * (3.0f * qy * qy - qr2);
                _quadrupoleMoment.yz() += child.summary().moment().yz() +
                                          child.summary().totalMass().mass() * (3.0f * qy * qz);
            }
            _quadrupoleMoment.zz() = -_quadrupoleMoment.xx() - _quadrupoleMoment.yy();
        }

        Quadrupole &moment() { return _quadrupoleMoment; }

        [[nodiscard]] const Quadrupole &moment() const { return _quadrupoleMoment; }

    };
}

#endif //N_BODY_QUADRUPOLEMASSSUMMARY_H
