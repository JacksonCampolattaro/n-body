//
// Created by Jackson Campolattaro on 1/9/23.
//

#ifndef N_BODY_LINEARBVH_H
#define N_BODY_LINEARBVH_H

#include <tbb/parallel_for_each.h>
#include "Tree.h"

#include "NBody/Simulation/Solvers/MortonSort.h"

#include "NBody/Physics/BoundingBox.h"

#include <NBody/Physics/Summaries/CenterOfMassSummary.h>
#include <NBody/Physics/Summaries/QuadrupoleMassSummary.h>
#include <NBody/Physics/Summaries/BoundingBoxSummary.h>

namespace NBody {

    template<SummaryType S>
    class LinearBVHNode : public NodeBase<LinearBVHNode<S>, BoundingBoxSummary<S>> {
    private:

        std::vector<LinearBVHNode<S>> _children;

    public:

        using SummaryType = BoundingBoxSummary<S>;

        using NodeBase<LinearBVHNode, SummaryType>::NodeBase;
        using NodeBase<LinearBVHNode, SummaryType>::contents;
        using NodeBase<LinearBVHNode, SummaryType>::isLeaf;
        using NodeBase<LinearBVHNode, SummaryType>::summary;

        [[nodiscard]] std::vector<LinearBVHNode> &children() { return _children; }

        [[nodiscard]] const std::vector<LinearBVHNode> &children() const { return _children; }

        [[nodiscard]] const BoundingBox &boundingBox() const { return summary().boundingBox(); }

        template<typename ViewType>
        void split(const ViewType &context) {

            // We need to determine where to split
            // Start by finding the most significant bit that changes between the start and the end of the coordinates
            // (because of our split criterion, we have a guarantee that they aren't the same)
            int msb = fls(context.template get<const MortonCode>(contents().front()) ^
                          context.template get<const MortonCode>(contents().back())) - 1;

            // Use std::upper_bound to perform a binary search and find where that bit changes
            MortonCode boundary = (1 << msb);
            auto split = std::upper_bound(contents().begin(), contents().end(), boundary,
                                          [&](MortonCode v, Entity i) {
                                              return v & context.template get<const MortonCode>(i);
                                          });

            auto low = std::span<Entity>{contents().begin(), split};
            auto high = std::span<Entity>{split, contents().end()};

            if (isLeaf()) {
                _children = {{{low}, {high}}};
            } else {
                children()[0].contents() = low;
                children()[1].contents() = high;
            }
        }

        void merge() {
            _children.clear();
        }

    };

    template<SummaryType S>
    class LinearBVH : public Tree<LinearBVHNode<S>> {
    public:

        using typename Tree<LinearBVHNode<S>>::Node;

        using Tree<Node>::Tree;
        using Tree<Node>::simulation;
        using Tree<Node>::root;
        using Tree<Node>::indices;

    protected:

        using Tree<Node>::depthSplit;
        using Tree<Node>::summarizeTreeTop;

    public:

        void build() override {

            // Assign morton codes to all active particles
            setMortonCodes(simulation(), outerBoundingBox<typename Node::SummaryType>(simulation()));

            // Sort the indices by the associated morton codes
            mortonSort(simulation().template view<const MortonCode>(), indices());

            // Build the tree
            // todo: the context used for splitting should be defined by the node type
            auto context = simulation().template view<const Position, const Mass, const MortonCode>();
            int preBuildDepth = 2;
            auto toBeRefined = depthSplit(preBuildDepth, context);
            tbb::parallel_for_each(toBeRefined, [&](std::reference_wrapper<typename LinearBVH::Node> node) {
                node.get().refine(std::numeric_limits<std::size_t>::max(),
                                  [&](const auto &n) {
                                      // Don't split if all entities in this node have the same morton code
                                      return context.template get<const MortonCode>(n.contents().front()) !=
                                             context.template get<const MortonCode>(n.contents().back());
                                  },
                                  context);
            });
            summarizeTreeTop(toBeRefined, context);
        }

    };

    using ActiveLinearBVH = LinearBVH<CenterOfMassSummary>;
    using QuadrupoleActiveLinearBVH = LinearBVH<QuadrupoleMassSummary>;

}

#endif //N_BODY_LINEARBVH_H
