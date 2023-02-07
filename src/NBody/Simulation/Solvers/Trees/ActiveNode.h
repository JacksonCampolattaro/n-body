//
// Created by Jackson Campolattaro on 1/12/23.
//

#ifndef N_BODY_MASSTREE_H
#define N_BODY_MASSTREE_H

#include "Tree.h"

#include <NBody/Simulation/Solvers/Trees/Summaries/CenterOfMassSummary.h>

namespace NBody {

    template<typename NodeImplementation>
    class ActiveNode : public NodeImplementation {
    private:

        CenterOfMassSummary _summary;

    public:

        using NodeImplementation::NodeImplementation;
        using NodeImplementation::isLeaf;
        using NodeImplementation::contents;
        using NodeImplementation::children;
        using NodeImplementation::boundingBox;

        CenterOfMassSummary &summary() { return _summary; }

        [[nodiscard]] const CenterOfMassSummary &summary() const { return _summary; }

        static std::vector<Entity> relevantEntities(Simulation &simulation) {
            return relevantEntities < CenterOfMassSummary > (simulation);
        }

        static BoundingBox outerBoundingBox(Simulation &simulation) {
            return outerBoundingBox < CenterOfMassSummary > (simulation);
        }

        static CenterOfMassSummary::Context constructionContext(Simulation &simulation) {
            return simulation.group<const Position, const Mass>();
        }

        template<typename ViewType>
        void summarize(const ViewType &context) {
            NodeImplementation::summarize(context);
            if (isLeaf())_summary.summarize(contents(), context);
            else _summary.summarize(children());
        }

    };
}

#endif //N_BODY_MASSTREE_H
