//
// Created by Jackson Campolattaro on 2/7/23.
//

#ifndef N_BODY_DUALSUMMARY_H
#define N_BODY_DUALSUMMARY_H

#include <NBody/Simulation/Solvers/Trees/Summary.h>

namespace NBody {

    using namespace Physics;

    class DualSummary {
    private:

        Position _centerOfMass{0.0f, 0.0f, 0.0f};
        Mass _totalMass{0.0f};

        Acceleration _acceleration{0.0f, 0.0f, 0.0f};

    public:

        using Context = Simulation;

        static Context &context(Simulation &simulation) { return simulation; }

    public:

        DualSummary() {}

        void summarize(const std::span<Entity> &entities, const Context &context) {

            _totalMass = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};

            for (const auto &entity: entities) {
                if (context.template all_of<Position, Mass>(entity)) {
                    auto entityPosition = context.template get<const Position>(entity);
                    auto entityMass = context.template get<const Mass>(entity).mass();
                    _totalMass.mass() += entityMass;
                    _centerOfMass = _centerOfMass + (entityMass * entityPosition);
                }
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

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

        [[nodiscard]] const Acceleration &acceleration() const { return _acceleration; }

        Acceleration &acceleration() { return _acceleration; }

    };
}

#endif //N_BODY_DUALSUMMARY_H
