//
// Created by Jackson Campolattaro on 2/28/23.
//

#ifndef N_BODY_MULTIPOLEMASSSUMMARY_H
#define N_BODY_MULTIPOLEMASSSUMMARY_H

#include <NBody/Physics/SummaryType.h>
#include <NBody/Physics/Multipole.h>

namespace NBody {

    using namespace Physics;

    template<std::size_t Order>
    class MultipoleMassSummary {
    private:

        Mass _totalMass{0.0f};
        Multipole<Order> _multipole{};

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

        MultipoleMassSummary() = default;

        template<typename Context>
        void summarize(const std::span<Entity> &entities, const Context &context) {

            _totalMass = Mass{0.0f};
            _multipole = Multipole<Order>{};

            // todo: dispatching shouldn't be done with constexpr, if possible

            // Total Mass
            if constexpr (Order >= 0) {
                _totalMass.mass() = std::transform_reduce(
                        entities.begin(), entities.end(),
                        0.0f, std::plus<>(),
                        [&](const auto &entity) {
                            return context.template get<const Mass>(entity).mass();
                        }
                );
            }

            // Center of mass
            if constexpr (Order >= 1) {
                _multipole.template matrix<1>() = std::transform_reduce(
                        entities.begin(), entities.end(),
                        glm::vec3{}, std::plus<>(),
                        [&](const auto &entity) {
                            return context.template get<const Position>(entity) *
                                   context.template get<const Mass>(entity).mass();
                        }
                ) / _totalMass.mass();
            }

            // Quadrupole Moment
            if constexpr (Order >= 2) {
                // todo
            }

            // Octupole Moment
            if constexpr (Order >= 3) {
                // todo
            }
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

#endif //N_BODY_MULTIPOLEMASSSUMMARY_H
