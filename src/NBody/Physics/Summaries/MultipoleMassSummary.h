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

            //            // Total Mass
            //            if constexpr (Order >= 0) {
            //                _totalMass.mass() = std::transform_reduce(
            //                        entities.begin(), entities.end(),
            //                        0.0f, std::plus<>(),
            //                        [&](const auto &entity) {
            //                            return context.template get<const Mass>(entity).mass();
            //                        }
            //                );
            //            }
            //
            //            // Center of mass
            //            if constexpr (Order >= 1) {
            //                _multipole.template tensor<1>() = std::transform_reduce(
            //                        entities.begin(), entities.end(),
            //                        glm::vec3{}, std::plus<>(),
            //                        [&](const auto &entity) {
            //                            return context.template get<const Position>(entity) *
            //                                   context.template get<const Mass>(entity).mass();
            //                        }
            //                ) / _totalMass.mass();
            //            }
            //
            //            // Quadrupole Moment
            //            if constexpr (Order >= 2) {
            //                _multipole.template tensor<2>() = std::transform_reduce(
            //                        entities.begin(), entities.end(),
            //                        SymmetricTensor3<2>{}, std::plus<>(),
            //                        [&](const auto &entity) {
            //                            auto entityMass = context.template get<const Mass>(entity).mass();
            //                            auto entityPosition = context.template get<const Position>(entity);
            //                            glm::vec3 d = entityPosition - centerOfMass();
            //                            return SymmetricTensor3<2>::cartesianPower(d).traceless() * entityMass * 3.0f;
            //                        }
            //                );
            //                _multipole.template tensor<2>().enforceTraceless();
            //                // todo
            //            }
            //
            //            // Octupole Moment
            //            if constexpr (Order >= 3) {
            //                // todo
            //            }
        }

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {

            // Find the total mass

            // Find each multipole order
            std::apply([](auto &&... tensors) {

                auto summarizeTensor = [&](const auto &t) {

                    spdlog::info("summarizing {}", sizeof t.flat());
                };

                (summarizeTensor(tensors), ...);

            }, _multipole.tensors());

            // Remove the total mass from the center of mass
            _multipole.template tensor<1>() /= _totalMass;

        }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

        SymmetricTensor3<1> &centerOfMass() { return _multipole.template tensor<1>(); }

        [[nodiscard]] const SymmetricTensor3<1> &centerOfMass() const { return _multipole.template tensor<1>(); }

        SymmetricTensor3<2> &moment() { return _multipole.template tensor<2>(); }

        [[nodiscard]] const SymmetricTensor3<2> &moment() const { return _multipole.template tensor<2>(); }
    };
}

#endif //N_BODY_MULTIPOLEMASSSUMMARY_H
