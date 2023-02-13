//
// Created by Jackson Campolattaro on 2/7/23.
//

#ifndef N_BODY_SUMMARYTYPE_H
#define N_BODY_SUMMARYTYPE_H

#include <concepts>
#include <span>

#include "NBody/Simulation/Simulation.h"

namespace NBody {

    template<typename T>
    concept IsEntityListSummarizer = requires(T &t,
                                              const std::span<Entity> &entities,
                                              const typename T::Context &context) {
        { t.summarize(entities, context) } -> std::convertible_to<void>;
    };

    template<typename T>
    concept IsContextProvider = requires(Simulation &simulation) {
        T::context(simulation);
    };

    template<typename T>
    concept SummaryType = IsEntityListSummarizer<T> &&
                          IsContextProvider<T> &&
                          std::is_default_constructible_v<T>;

    template<SummaryType S>
    decltype(auto) positionsView(Simulation &simulation) {
        if constexpr(std::is_same_v<typename S::Context, Simulation>)
            return simulation.template view<const Position>();
        else
            return S::context(simulation);
    }

    template<SummaryType S>
    static std::vector<Entity> relevantEntities(Simulation &simulation) {
        const auto &&context = positionsView<S>(simulation);
        return {context.begin(), context.end()};
    }

    template<SummaryType S>
    static BoundingBox outerBoundingBox(Simulation &simulation) {
        BoundingBox bbox;
        for (Entity e: positionsView<S>(simulation)) {
            bbox.min() = glm::min((glm::vec3) bbox.min(), simulation.get<const Position>(e));
            bbox.max() = glm::max((glm::vec3) bbox.max(), simulation.get<const Position>(e));
        };
        return bbox;
    }
}

#endif //N_BODY_SUMMARYTYPE_H
