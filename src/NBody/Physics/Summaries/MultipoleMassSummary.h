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

            // Center of Mass
            _multipole.template tensor<1>() = std::transform_reduce(
                    entities.begin(), entities.end(),
                    glm::vec3{}, std::plus<>(),
                    [&](const auto &entity) {
                        const auto &position = context.template get<const Position>(entity);
                        const auto &mass = context.template get<const Mass>(entity).mass();
                        return (glm::vec3) position * mass;
                    }
            ) / _totalMass.mass();

            // Quadrupole
            _multipole.template tensor<2>() = std::transform_reduce(
                    entities.begin(), entities.end(),
                    SymmetricTensor3<2>{}, std::plus<>(),
                    [&](const auto &entity) {
                        const auto &position = context.template get<const Position>(entity);
                        const auto &mass = context.template get<const Mass>(entity).mass();
                        glm::vec3 d = position - centerOfMass();
                        return SymmetricTensor3<2>::cartesianPower(d).traceless() * mass * 3.0f;
                    }
            );
            _multipole.template tensor<2>().enforceTraceless();
        }

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {

            // Total Mass
            _totalMass.mass() = std::transform_reduce(
                    childNodes.begin(), childNodes.end(),
                    0.0f, std::plus<>(),
                    [&](const auto &childNode) {
                        return childNode.summary().totalMass().mass();
                    }
            );

            // Center of Mass
            _multipole.template tensor<1>() = std::transform_reduce(
                    childNodes.begin(), childNodes.end(),
                    glm::vec3{}, std::plus<>(),
                    [&](const auto &childNode) {
                        const auto &mass = childNode.summary().totalMass().mass();
                        const auto &position = childNode.summary().centerOfMass();
                        return (glm::vec3) position * mass;
                    }
            ) / _totalMass.mass();

            // Quadrupole
            _multipole.template tensor<2>() = std::transform_reduce(
                    childNodes.begin(), childNodes.end(),
                    SymmetricTensor3<2>{}, std::plus<>(),
                    [&](const auto &childNode) {
                        auto &position = childNode.summary().centerOfMass();
                        auto &mass = childNode.summary().totalMass().mass();
                        glm::vec3 d = position - centerOfMass();
                        return SymmetricTensor3<2>::cartesianPower(d).traceless() * mass * 3.0f +
                               childNode.summary().moment();
                    }
            );
            _multipole.template tensor<2>().enforceTraceless();
        }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

        glm::vec3 &centerOfMass() { return _multipole.template tensor<1>(); }

        [[nodiscard]] const glm::vec3 &centerOfMass() const { return _multipole.template tensor<1>(); }

        SymmetricTensor3<2> &moment() { return _multipole.template tensor<2>(); }

        [[nodiscard]] const SymmetricTensor3<2> &moment() const { return _multipole.template tensor<2>(); }

        friend std::ostream &operator<<(std::ostream &out, const MultipoleMassSummary<Order> &s) {
            return out << "(" << (s.moment() == SymmetricTensor3<2>{} ? "0..." : "") << ")";
        }
    };
}

#endif //N_BODY_MULTIPOLEMASSSUMMARY_H
