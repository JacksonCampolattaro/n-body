//
// Created by Jackson Campolattaro on 1/12/23.
//

#ifndef N_BODY_PASSIVENODE_H
#define N_BODY_PASSIVENODE_H

#include "Tree.h"

#include <NBody/Simulation/Solvers/Trees/Summaries/AccelerationSummary.h>

namespace NBody {

    template<typename NodeImplementation>
    class PassiveNode : public NodeImplementation {
    private:

        AccelerationSummary _summary;

    public:

        using NodeImplementation::NodeImplementation;
        using NodeImplementation::isLeaf;
        using NodeImplementation::contents;
        using NodeImplementation::children;

        AccelerationSummary &summary() { return _summary; }

        [[nodiscard]] const AccelerationSummary &summary() const { return _summary; }

        template<typename ViewType>
        void summarize(const ViewType &context) {
            NodeImplementation::summarize(context);
            if (isLeaf())_summary.summarize(contents(), context);
            else _summary.summarize(children());
        }

        static std::vector<Entity> relevantEntities(Simulation &simulation) {
            return relevantEntities < AccelerationSummary > (simulation);
        }

        static BoundingBox outerBoundingBox(Simulation &simulation) {
            return outerBoundingBox < AccelerationSummary > (simulation);
        }

        static AccelerationSummary::Context constructionContext(Simulation &simulation) {
            return AccelerationSummary::context(simulation);
        }

        [[nodiscard]] const Acceleration &acceleration() const { return _summary.acceleration(); }

        Acceleration &acceleration() { return _summary.acceleration(); }

        // todo: this should be moved elsewhere
        void collapseAccelerations(const entt::basic_view<entt::entity, entt::exclude_t<>, Acceleration> &context,
                                   Acceleration netAcceleration = {0.0f, 0.0f, 0.0f}) const {

            netAcceleration += (glm::vec3) acceleration();

            if (isLeaf()) {

                // When we reach a leaf node, apply the local force to all contained points
                for (auto i: contents())
                    context.get<Acceleration>(i) += (glm::vec3) netAcceleration;

            } else {

                // Descend the tree recursively, keeping track of the net acceleration over the current region
                for (const auto &child: children())
                    child.collapseAccelerations(context, netAcceleration);

            }
        }
    };
}

#endif //N_BODY_PASSIVENODE_H
