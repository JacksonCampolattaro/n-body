//
// Created by Jackson Campolattaro on 1/9/23.
//

#ifndef N_BODY_LINEARBVH_H
#define N_BODY_LINEARBVH_H

#include <tbb/parallel_for_each.h>
#include "TreeBase.h"

#include "NBody/Simulation/Solvers/MortonSort.h"

#include "NBody/Simulation/BoundingBox.h"

#include <NBody/Simulation/Solvers/Trees/Summaries/CenterOfMassSummary.h>

namespace NBody {

    template<typename LinearBVHNodeImplementation, Summary SummaryType>
    class LinearBVHNodeBase : public NodeBase<LinearBVHNodeImplementation, SummaryType> {
    private:

        std::vector<LinearBVHNodeImplementation> _children;

        BoundingBox _boundingBox{};

    public:

        using NodeBase<LinearBVHNodeImplementation, SummaryType>::NodeBase;
        using NodeBase<LinearBVHNodeImplementation, SummaryType>::contents;
        using NodeBase<LinearBVHNodeImplementation, SummaryType>::isLeaf;

        [[nodiscard]] std::vector<LinearBVHNodeImplementation> &children() { return _children; }

        [[nodiscard]] const std::vector<LinearBVHNodeImplementation> &children() const { return _children; }

        [[nodiscard]] const BoundingBox &boundingBox() const { return _boundingBox; }

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

        template<typename Context>
        void summarize(const Context &context) {
            NodeBase<LinearBVHNodeImplementation, SummaryType>::summarize(context);

            _boundingBox = BoundingBox{};

            if (isLeaf()) {

                for (const auto &entity: contents()) {
                    auto entityPosition = context.template get<const Position>(entity);
                    auto entityMass = context.template get<const Mass>(entity).mass();
                    _boundingBox.min() = glm::min((glm::vec3) entityPosition, (glm::vec3) _boundingBox.min());
                    _boundingBox.max() = glm::max((glm::vec3) entityPosition, (glm::vec3) _boundingBox.max());
                }

            } else {

                for (const auto &child: children()) {
                    _boundingBox.min() = glm::min((glm::vec3) child.boundingBox().min(),
                                                  (glm::vec3) _boundingBox.min());
                    _boundingBox.max() = glm::max((glm::vec3) child.boundingBox().max(),
                                                  (glm::vec3) _boundingBox.max());
                }

            }

        }

        void merge() {
            _children.clear();
        }

    };

    class LinearBVHNode : public LinearBVHNodeBase<LinearBVHNode, CenterOfMassSummary> {
    public:
        using LinearBVHNodeBase::LinearBVHNodeBase;

        static entt::basic_group<
                entt::entity, entt::exclude_t<>,
                entt::get_t<>,
                const NBody::Physics::Position,
                const NBody::Physics::Mass,
                const MortonCode
        > constructionContext(Simulation &simulation) {
            return simulation.group<const Position, const Mass, const MortonCode>();
        }
    };

    class LinearBVH : public TreeBase<LinearBVHNode> {
    public:

        explicit LinearBVH(Simulation &simulation) : TreeBase<LinearBVHNode>(simulation) {}

        void build() override {

            // Assign morton codes to all active particles
            setMortonCodes(simulation(), outerBoundingBox<LinearBVHNode::SummaryType>(simulation()));

            // Sort the indices by the associated morton codes
            mortonSort(simulation().view<const MortonCode>(), indices());

            // Build the tree
            // todo: the context should somehow be defined by the summary type
            auto context = simulation().view<const Position, const Mass, const MortonCode>();
            int preBuildDepth = 2;
            auto toBeRefined = depthSplit(root(), preBuildDepth, context);
            tbb::parallel_for_each(toBeRefined, [&](std::reference_wrapper<typename LinearBVH::Node> node) {
                node.get().refine(std::numeric_limits<std::size_t>::max(),
                                  [&](const auto &n) {
                                      // Don't split if all entities in this node have the same morton code
                                      return context.get<const MortonCode>(n.contents().front()) !=
                                             context.get<const MortonCode>(n.contents().back());
                                  },
                                  context);
            });
            summarizeTreeTop(root(), toBeRefined, context);

            // todo: maybe non-parallel construction should be available as an option?
            //            root().refine(
            //                    std::numeric_limits<std::size_t>::max(),
            //                    [&](const auto &n) {
            //                        // Don't split if all entities in this node have the same morton code
            //                        return context.get<const MortonCode>(n.contents().front()) !=
            //                               context.get<const MortonCode>(n.contents().back());
            //                    },
            //                    context
            //            );
        }

    };

}

#endif //N_BODY_LINEARBVH_H
