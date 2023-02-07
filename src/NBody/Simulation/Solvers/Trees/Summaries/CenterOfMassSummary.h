//
// Created by Jackson Campolattaro on 2/7/23.
//

#ifndef N_BODY_CENTEROFMASSSUMMARY_H
#define N_BODY_CENTEROFMASSSUMMARY_H

#include <NBody/Simulation/Solvers/Trees/Summary.h>

namespace NBody {

    using namespace Physics;

    class CenterOfMassSummary {
    private:

        Position _centerOfMass{0.0f, 0.0f, 0.0f};
        Mass _totalMass{0.0f};

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

        CenterOfMassSummary() {}

        CenterOfMassSummary(const std::span<Entity> &entities, const Context &context) {

            _totalMass = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};

            for (const auto &entity: entities) {
                auto entityMass = context.template get<const Mass>(entity).mass();
                _totalMass.mass() += entityMass;
                _centerOfMass = _centerOfMass + (entityMass * context.template get<const Position>(entity));
            }
            _centerOfMass = _centerOfMass / _totalMass.mass();
        }

        template<typename NodeList>
        explicit CenterOfMassSummary(const NodeList &childNodes) {

            _totalMass = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};

            for (const auto &child: childNodes) {
                _totalMass.mass() += child.summary().totalMass().mass();
                _centerOfMass = _centerOfMass + (child.summary().centerOfMass() * child.summary().totalMass().mass());
            }
            _centerOfMass = _centerOfMass / _totalMass.mass();
        }

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

    };
}

#endif //N_BODY_CENTEROFMASSSUMMARY_H
