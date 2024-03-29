//
// Created by Jackson Campolattaro on 2/7/23.
//

#ifndef N_BODY_CENTEROFMASSSUMMARY_H
#define N_BODY_CENTEROFMASSSUMMARY_H

#include "SummaryType.h"

namespace NBody {

    using namespace Physics;

    class CenterOfMassSummary {
    private:

        Position _centerOfMass{0.0f, 0.0f, 0.0f};
        Mass _totalMass{0.0f};

    public:

        static auto context(Simulation &simulation) {
            return simulation.group<const Position, const Mass>();
        }

    public:

        CenterOfMassSummary() = default;

        template<typename C>
        void summarize(std::span<const Entity> entities, const C &context) {

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
        void summarize(const NodeList &childNodes) {

            _totalMass = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};

            for (const auto &child: childNodes) {
                _totalMass.mass() += child.summary().totalMass().mass();
                _centerOfMass = _centerOfMass + (child.summary().centerOfMass() * child.summary().totalMass().mass());
            }
            _centerOfMass = _centerOfMass / _totalMass.mass();
        }

        [[nodiscard]] Position &centerOfMass() { return _centerOfMass; }

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

    };
}

#endif //N_BODY_CENTEROFMASSSUMMARY_H
