//
// Created by Jackson Campolattaro on 2/12/23.
//

#ifndef N_BODY_QUADRUPOLEMASSSUMMARY_H
#define N_BODY_QUADRUPOLEMASSSUMMARY_H

#include <NBody/Physics/Quadrupole.h>
#include <NBody/Simulation/Solvers/Trees/SummaryType.h>

namespace NBody {

    using namespace Physics;

    class QuadrupoleMassSummary {
    private:

        Mass _totalMass{0.0f};
        Position _centerOfMass{0.0f, 0.0f, 0.0f};
        Quadrupole _quadrupoleMoment{};

    public:

        using Context = entt::basic_group<
                entt::entity, entt::exclude_t<>,
                entt::get_t<>,
                const Position,
                const Mass
        >;

        static Context context(Simulation &simulation) {
            return simulation.group<const Position, const Mass>();
        }

    public:

        QuadrupoleMassSummary() {}

        template<typename C>
        void summarize(const std::span<Entity> &entities, const C &context) {

            _totalMass = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};
            _quadrupoleMoment = {};

            for (const auto &entity: entities) {
                auto entityMass = context.template get<const Mass>(entity).mass();
                auto entityPosition = context.template get<const Position>(entity);
                _totalMass.mass() += entityMass;
                _centerOfMass = _centerOfMass + (entityMass * entityPosition);
            }
            _centerOfMass = _centerOfMass / _totalMass.mass();

            for (const auto &entity: entities) {
                auto entityMass = context.template get<const Mass>(entity).mass();
                auto entityPosition = context.template get<const Position>(entity);

                float qx = entityPosition.x - _centerOfMass.x;
                float qy = entityPosition.y - _centerOfMass.y;
                float qz = entityPosition.z - _centerOfMass.z;
                float qr2 = qx * qx + qy * qy + qz * qz;

                _quadrupoleMoment.xx() += entityMass * (3.0f * qx * qx - qr2);
                _quadrupoleMoment.xy() += entityMass * (3.0f * qx * qy);
                _quadrupoleMoment.xz() += entityMass * (3.0f * qx * qz);
                _quadrupoleMoment.yy() += entityMass * (3.0f * qy * qy - qr2);
                _quadrupoleMoment.yz() += entityMass * (3.0f * qy * qz);
            }
            _quadrupoleMoment.zz() = -_quadrupoleMoment.xx() -_quadrupoleMoment.yy();
        }

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {

            _totalMass = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};
            _quadrupoleMoment = {};

            for (const auto &child: childNodes) {
                _totalMass.mass() += child.summary().totalMass().mass();
                _centerOfMass = _centerOfMass + (child.summary().centerOfMass() * child.summary().totalMass().mass());
            }
            _centerOfMass = _centerOfMass / _totalMass.mass();

            for (const auto &child: childNodes) {

                float qx = child.summary().centerOfMass().x - _centerOfMass.x;
                float qy = child.summary().centerOfMass().y - _centerOfMass.y;
                float qz = child.summary().centerOfMass().z - _centerOfMass.z;
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
            _quadrupoleMoment.zz() = -_quadrupoleMoment.xx() -_quadrupoleMoment.yy();
        }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

        Position &centerOfMass() { return _centerOfMass; }

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

        Quadrupole &moment() { return _quadrupoleMoment; }

        [[nodiscard]] const Quadrupole &moment() const { return _quadrupoleMoment; }

    };
}

#endif //N_BODY_QUADRUPOLEMASSSUMMARY_H
