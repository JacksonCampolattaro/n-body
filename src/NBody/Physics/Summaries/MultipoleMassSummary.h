//
// Created by Jackson Campolattaro on 2/28/23.
//

#ifndef N_BODY_MULTIPOLEMASSSUMMARY_H
#define N_BODY_MULTIPOLEMASSSUMMARY_H

#include "SummaryType.h"
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
            //_multipole.enforceTraceless();
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
                        auto moment = (MultipoleMoment<Order>{offset} * mass) + childNode.summary().moment();

                        // todo: Is there a cleaner way to handle the additional terms?
                        if constexpr (Order >= 3) {
                            moment.template tensor<3>() += SymmetricTensor3<3>::sumOfOuterProducts(
                                    childNode.summary().moment().template tensor<2>(),
                                    offset
                            );
                        }
                        if constexpr (Order >= 4) {
                            // todo: is this correct?
                            moment.template tensor<4>() += SymmetricTensor3<4>::sumOfOuterProducts(
                                    childNode.summary().moment().template tensor<3>(),
                                    offset
                            );
                            moment.template tensor<4>() += SymmetricTensor3<4>::sumOfOuterProducts(
                                    SymmetricTensor3<3>::outerProduct(
                                            childNode.summary().moment().template tensor<2>(),
                                            offset
                                    ),
                                    offset
                            );
                        }
                        if constexpr (Order >= 5) {
                            // todo
                        }
                        // etc.

                        return moment;
                    }
            );
            //_multipole.enforceTraceless();
        }

        MultipoleMoment<Order> &moment() { return _multipole; }

        [[nodiscard]] const MultipoleMoment<Order> &moment() const { return _multipole; }

        friend std::ostream &operator<<(std::ostream &out, const MultipoleMassSummary<Order> &s) {
            return out << "()";
        }
    };

    using QuadrupoleMassSummary = MultipoleMassSummary<2>;
    using OctupoleMassSummary = MultipoleMassSummary<3>;
    using HexadecupoleMassSummary = MultipoleMassSummary<4>;
}

#endif //N_BODY_MULTIPOLEMASSSUMMARY_H
