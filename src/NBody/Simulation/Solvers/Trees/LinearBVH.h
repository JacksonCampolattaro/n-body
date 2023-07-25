//
// Created by Jackson Campolattaro on 1/9/23.
//

#ifndef N_BODY_LINEARBVH_H
#define N_BODY_LINEARBVH_H

#include "Tree.h"

#include "NBody/Simulation/Solvers/MortonSort.h"

#include "NBody/Physics/BoundingBox.h"

#include <NBody/Physics/Summaries/CenterOfMassSummary.h>
#include <NBody/Physics/Summaries/BoundingBoxSummary.h>

#include <tbb/parallel_for_each.h>
#include <strings.h>

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

        [[nodiscard]] Position center() const { return summary().boundingBox().center(); }

        template<typename ViewType>
        void split(const ViewType &context) {

            // We need to determine where to split
            // Start by finding the most significant bit that changes between the start and the end of the coordinates
            // (because of our split criterion, we have a guarantee that they aren't the same)
            // todo
            int msb = (8 * sizeof(MortonCode)) -
                      std::countl_zero(context.template get<const MortonCode>(contents().front()) ^
                                       context.template get<const MortonCode>(contents().back())) - 1;

            // Use std::upper_bound to perform a binary search and find where that bit changes
            MortonCode boundary = (1 << msb);
            auto split = std::upper_bound(contents().begin(), contents().end(), boundary,
                                          [&](MortonCode v, Entity i) {
                                              return (v & context.template get<const MortonCode>(i)) != 0;
                                          });

            auto low = std::span<Entity>{contents().begin(), split};
            auto high = std::span<Entity>{split, contents().end()};
            assert(!low.empty() && !high.empty());

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
    private:

        int _maxLeafSize = 32;

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
            recursiveParallelMortonSort(simulation().template view<const MortonCode>(), indices());

            auto context = simulation().template view<const Position, const Mass, const MortonCode>();
            auto splitCriterion = [&](const Node &n) -> bool {
                // Don't split if all entities in this node have the same morton code
                return n.contents().size() > _maxLeafSize &&
                       context.template get<const MortonCode>(n.contents().front()) !=
                       context.template get<const MortonCode>(n.contents().back());
            };

            // Build the tree
            // todo: the context used for splitting should be defined by the node type
            auto toBeRefined = this->loadBalancedSplit(32, splitCriterion, context);
            tbb::parallel_for_each(toBeRefined, [&](std::reference_wrapper<typename LinearBVH::Node> node) {
                node.get().refine(std::numeric_limits<std::size_t>::max(),
                                  splitCriterion,
                                  context);
            });
            summarizeTreeTop(toBeRefined, context);
        }

        int &maxLeafSize() { return _maxLeafSize; }

        [[nodiscard]] const int &maxLeafSize() const { return _maxLeafSize; }

    };


    template<std::size_t Order>
    using MultipoleActiveLinearBVH = LinearBVH<MultipoleMassSummary<Order>>;

    using ActiveLinearBVH = LinearBVH<CenterOfMassSummary>;
    using QuadrupoleActiveLinearBVH = MultipoleActiveLinearBVH<2>;

}

#endif //N_BODY_LINEARBVH_H
