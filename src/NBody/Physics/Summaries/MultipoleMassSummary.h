//
// Created by Jackson Campolattaro on 2/28/23.
//

#ifndef N_BODY_MULTIPOLEMASSSUMMARY_H
#define N_BODY_MULTIPOLEMASSSUMMARY_H

#include <NBody/Physics/SummaryType.h>
#include <NBody/Physics/MultipoleMoment.h>

namespace NBody {

    using namespace Physics;

    template<std::size_t Order>
    class MultipoleMassSummary {
    private:

        Mass _totalMass{0.0f};
        MultipoleMoment<Order> _multipole{};

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

            // Total Mass
            _totalMass.mass() = std::transform_reduce(
                    entities.begin(), entities.end(),
                    0.0f, std::plus<>(),
                    [&](const auto &entity) {
                        return context.template get<const Mass>(entity).mass();
                    }
            );

            // Multipole
            _multipole = std::transform_reduce(
                    entities.begin(), entities.end(),
                    MultipoleMoment<Order>{}, std::plus<>(),
                    [&](const auto &entity) {
                        const auto &position = context.template get<const Position>(entity);
                        const auto &mass = context.template get<const Mass>(entity);
                        auto v = MultipoleMoment<Order>{position};
                        v *= mass;
                        return v;
                    }
            ) / _totalMass.mass();

            // todo: should this be done here?
            _multipole.applyCoefficients();
            //_multipole.template tensor<2>().enforceTraceless();
        }

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {


            Mass totalMass{0.0f};
            Position centerOfMass{0.0f, 0.0f, 0.0f};
            for (const auto &node: childNodes) {
                auto nodeMass = node.summary().totalMass().mass();
                totalMass.mass() += nodeMass;
                centerOfMass = centerOfMass + (nodeMass * node.summary().centerOfMass());
            }
            centerOfMass = centerOfMass / totalMass.mass();

            // Total Mass
            _totalMass.mass() = std::transform_reduce(
                    childNodes.begin(), childNodes.end(),
                    0.0f, std::plus<>(),
                    [&](const auto &childNode) {
                        return childNode.summary().totalMass().mass();
                    }
            );

            // Multipole
            _multipole = std::transform_reduce(
                    childNodes.begin(), childNodes.end(),
                    MultipoleMoment<Order>{}, std::plus<>(),
                    [&](const auto &childNode) {
                        auto &position = childNode.summary().centerOfMass();
                        return MultipoleMoment<Order>{position} *
                               childNode.summary().totalMass();
                    }
            ) / _totalMass.mass();


//            if (_multipole.template tensor<1>() != centerOfMass)
//                std::cout << (Position) (centerOfMass / totalMass.mass()) <<
//                          " != " << (Position) _multipole.template tensor<1>() << std::endl;

            // todo: should this be done here?
            _multipole.applyCoefficients();
            //_multipole.template tensor<2>().enforceTraceless();
        }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

        glm::vec3 &centerOfMass() { return _multipole.template tensor<1>(); }

        [[nodiscard]] const glm::vec3 &centerOfMass() const { return _multipole.template tensor<1>(); }

        SymmetricTensor3<2> &moment() { return _multipole.template tensor<2>(); }

        [[nodiscard]] const SymmetricTensor3<2> &moment() const { return _multipole.template tensor<2>(); }
    };
}

#endif //N_BODY_MULTIPOLEMASSSUMMARY_H
