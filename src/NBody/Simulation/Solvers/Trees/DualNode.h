//
// Created by Jackson Campolattaro on 1/26/23.
//

#ifndef N_BODY_DUALNODE_H
#define N_BODY_DUALNODE_H

#include "Tree.h"

#include <NBody/Simulation/Solvers/Trees/Summaries/DualSummary.h>

namespace NBody {

    template<typename NodeImplementation>
    class DualNode : public NodeImplementation {
    private:

        DualSummary _summary;

    public:

        using NodeImplementation::NodeImplementation;
        using NodeImplementation::isLeaf;
        using NodeImplementation::contents;
        using NodeImplementation::children;
        using NodeImplementation::boundingBox;

        DualSummary &summary() { return _summary; }

        [[nodiscard]] const DualSummary &summary() const { return _summary; }

        static std::vector<Entity> relevantEntities(Simulation &simulation) {
            return relevantEntities < DualSummary > (simulation);
        }

        static BoundingBox outerBoundingBox(Simulation &simulation) {
            return outerBoundingBox < DualSummary > (simulation);
        }

        static Simulation &constructionContext(Simulation &simulation) {
            return DualSummary::context(simulation);
        }

        template<typename ViewType>
        void summarize(const ViewType &context) {
            NodeImplementation::summarize(context);
            if (isLeaf())_summary.summarize(contents(), context);
            else _summary.summarize(children());
        }

        void collapseAccelerations(const entt::basic_view<entt::entity, entt::exclude_t<>, Acceleration> &context,
                                   Acceleration netAcceleration = {0.0f, 0.0f, 0.0f}) const {

            netAcceleration += (glm::vec3) summary().acceleration();

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

#endif //N_BODY_DUALNODE_H
