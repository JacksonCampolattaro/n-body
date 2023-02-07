//
// Created by Jackson Campolattaro on 2/7/23.
//

#ifndef N_BODY_SUMMARY_H
#define N_BODY_SUMMARY_H

#include <concepts>
#include <span>

#include <NBody/Simulation/Simulation.h>

namespace NBody {

    template<typename T>
    concept ConstructibleFromEntitiesAndContext = requires(const std::span<Entity> &entities,
                                                           const typename T::Context &context) {
        T{entities, context};
    };

    template<typename T>
    concept ConstructibleFromChildNodes = requires(const std::vector<OctreeNode> &childNodes) {
        T{childNodes};
    };

    template<typename T>
    concept IsContextProvider = requires(Simulation &simulation) {
        { T::context(simulation) } -> std::convertible_to<typename T::Context>;
    };

    template<typename T>
    concept Summary = ConstructibleFromEntitiesAndContext<T> &&
                      ConstructibleFromChildNodes<T> &&
                      std::is_default_constructible_v<T> &&
                      IsContextProvider<T>;

    template<Summary S>
    decltype(auto) positionsView(Simulation &simulation) {
        if constexpr(std::is_same_v<typename S::Context, Simulation>)
            return simulation.template group<const Position>();
        else
            return S::context(simulation);
    }

    template<Summary S>
    static std::vector<Entity> relevantEntities(Simulation &simulation) {
        const auto &&context = positionsView<S>(simulation);
        return {context.begin(), context.end()};
    }

    template<Summary S>
    static BoundingBox boundingBox(Simulation &simulation) {
        BoundingBox bbox;
        positionsView<S>(simulation).each([&](Entity e) {
            bbox.min() = glm::min((glm::vec3) bbox.min(), simulation.get<const Position>(e));
            bbox.max() = glm::max((glm::vec3) bbox.max(), simulation.get<const Position>(e));
        });
        return bbox;
    }
}

#endif //N_BODY_SUMMARY_H
