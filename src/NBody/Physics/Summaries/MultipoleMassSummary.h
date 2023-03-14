//
// Created by Jackson Campolattaro on 2/28/23.
//

#ifndef N_BODY_MULTIPOLEMASSSUMMARY_H
#define N_BODY_MULTIPOLEMASSSUMMARY_H

#include <NBody/Physics/SummaryType.h>
#include <NBody/Physics/MultipoleMoment.h>
#include <NBody/Physics/Summaries/CenterOfMassSummary.h>

// Loosely follows:
// https://github.com/weiguangcui/Gadget4/blob/123a58f2501b7e7becfaf1ceccd563998c13e124/src/gravtree/gravtree_build.cc

namespace NBody {

    using namespace Physics;

    template<std::size_t Order>
    class MultipoleMassSummary : protected CenterOfMassSummary {
    private:

        MultipoleMoment<Order> _multipole{};

    public:

        using CenterOfMassSummary::Context;
        using CenterOfMassSummary::context;

        using CenterOfMassSummary::CenterOfMassSummary;
        using CenterOfMassSummary::totalMass;
        using CenterOfMassSummary::centerOfMass;

    public:

        MultipoleMassSummary() = default;

        template<typename Context>
        void summarize(const std::span<Entity> &entities, const Context &context) {
            CenterOfMassSummary::summarize(entities, context);

            _multipole = std::transform_reduce(
                    entities.begin(), entities.end(),
                    MultipoleMoment<Order>{}, std::plus<>(),
                    [&](const auto &entity) {
                        const auto &position = context.template get<const Position>(entity);
                        const auto &mass = context.template get<const Mass>(entity);
                        glm::vec3 offset = position - centerOfMass();
                        return MultipoleMoment<Order>{offset} * mass;
                    }
            );
            _multipole.enforceTraceless();
        }

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {
            CenterOfMassSummary::summarize(childNodes);

            _multipole = std::transform_reduce(
                    childNodes.begin(), childNodes.end(),
                    MultipoleMoment<Order>{}, std::plus<>(),
                    [&](const auto &childNode) {
                        const auto &position = childNode.summary().centerOfMass();
                        const auto &mass = childNode.summary().totalMass();
                        glm::vec3 offset = position - centerOfMass();
                        // todo: there's also a few other terms involved here, for octupole and higher!
                        return (MultipoleMoment<Order>{offset} * mass) + childNode.summary().moment();
                    }
            );
            _multipole.enforceTraceless();
        }

        MultipoleMoment<Order> &moment() { return _multipole; }

        [[nodiscard]] const MultipoleMoment<Order> &moment() const { return _multipole; }

        friend std::ostream &operator<<(std::ostream &out, const MultipoleMassSummary<Order> &s) {
            return out << "()";
        }
    };
}

#endif //N_BODY_MULTIPOLEMASSSUMMARY_H
