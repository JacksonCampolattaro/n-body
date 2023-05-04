//
// Created by Jackson Campolattaro on 7/3/22.
//

#ifndef N_BODY_OCTREE_H
#define N_BODY_OCTREE_H

#include <span>
#include <set>
#include <stack>

#include <spdlog/spdlog.h>
#include <tbb/parallel_for_each.h>

#include <NBody/Simulation/Simulation.h>

#include "NBody/Physics/Rule.h"

#include <NBody/Simulation/Solvers/Trees/Tree.h>
#include <NBody/Physics/Summaries/CenterOfMassSummary.h>
#include <NBody/Physics/Summaries/MultipoleMassSummary.h>
#include <NBody/Physics/Summaries/MultipoleDualSummary.h>
#include <NBody/Physics/Summaries/AccelerationSummary.h>
#include <NBody/Physics/Summaries/DualSummary.h>
#include <NBody/Physics/Summaries/EmptySummary.h>

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::Mass;

namespace {

    template<int Dimension, typename ViewType>
    [[nodiscard]] auto partition(std::span<NBody::Entity> &entities,
                                 const ViewType &positions,
                                 const NBody::Physics::Position &center) {

        auto c = std::partition(entities.begin(), entities.end(), [&](NBody::Entity entity) {
            return (positions.template get<const Position>(entity))[Dimension] < center[Dimension];
        });

        std::span less{&*entities.begin(), static_cast<std::size_t>(c - entities.begin())};
        std::span more{&*c, static_cast<std::size_t>(entities.end() - c)};
        assert(less.size() + more.size() == entities.size());

        if constexpr (Dimension == 0) {

            // Base case
            return std::tuple{less, more};

        } else {

            // This can be done with std::arrays, using something like:
            // https://stackoverflow.com/questions/10604794/convert-stdtuple-to-stdarray-c11
            // Or this:
            // https://stackoverflow.com/questions/45287195/combine-two-or-more-arrays-of-different-size-to-one-array-at-compiletime
            return std::tuple_cat(partition<Dimension - 1>(less, positions, center),
                                  partition<Dimension - 1>(more, positions, center));
        }

    }

}

namespace NBody {

    template<SummaryType SummaryType>
    class OctreeNode : public NodeBase<OctreeNode<SummaryType>, SummaryType> {
    private:

        std::vector<OctreeNode> _children;

        Position _center{0.0f, 0.0f, 0.0f};
        float _sideLength{1.0f};

    public:

        using NodeBase<OctreeNode, SummaryType>::NodeBase;
        using NodeBase<OctreeNode, SummaryType>::contents;
        using NodeBase<OctreeNode, SummaryType>::isLeaf;

        OctreeNode(std::span<Entity> contents, const Position &center, float sideLength) :
                NodeBase<OctreeNode, SummaryType>(contents),
                _center(center), _sideLength(sideLength) {}

        [[nodiscard]] std::vector<OctreeNode> &children() { return _children; }

        [[nodiscard]] const std::vector<OctreeNode> &children() const { return _children; }

        [[nodiscard]] BoundingBox boundingBox() const {
            glm::vec3 dimensions{_sideLength, _sideLength, _sideLength};
            return {
                    {_center - dimensions / 2.0f},
                    {_center + dimensions / 2.0f}
            };
        }

        template<typename Context>
        void split(const Context &context) {

            std::span<NBody::Entity>
                    xyz000,
                    xyz100,
                    xyz010,
                    xyz110,
                    xyz001,
                    xyz101,
                    xyz011,
                    xyz111;

            std::tie(
                    xyz000,
                    xyz100,
                    xyz010,
                    xyz110,
                    xyz001,
                    xyz101,
                    xyz011,
                    xyz111
            ) = partition<2>(contents(), context, _center);

            // Initialize 8 child nodes
            float childSideLength = sideLength() / 2.0f;
            float childOffset = childSideLength / 2.0f;
            _children = {{
                                 {xyz000,
                                  center() + glm::vec3{-childOffset, -childOffset, -childOffset},
                                  childSideLength},
                                 {xyz001,
                                  center() + glm::vec3{-childOffset, -childOffset, childOffset},
                                  childSideLength},
                                 {xyz010,
                                  center() + glm::vec3{-childOffset, childOffset, -childOffset},
                                  childSideLength},
                                 {xyz011,
                                  center() + glm::vec3{-childOffset, childOffset, childOffset},
                                  childSideLength},
                                 {xyz100,
                                  center() + glm::vec3{childOffset, -childOffset, -childOffset},
                                  childSideLength},
                                 {xyz101,
                                  center() + glm::vec3{childOffset, -childOffset, childOffset},
                                  childSideLength},
                                 {xyz110,
                                  center() + glm::vec3{childOffset, childOffset, -childOffset},
                                  childSideLength},
                                 {xyz111,
                                  center() + glm::vec3{childOffset, childOffset, childOffset},
                                  childSideLength}
                         }};
        }

        void merge() {
            _children.clear();
        }

        [[nodiscard]] Position &center() { return _center; }

        [[nodiscard]] const Position &center() const { return _center; }

        [[nodiscard]] const float &sideLength() const { return _sideLength; }

        [[nodiscard]] float &sideLength() { return _sideLength; }

    };

    template<SummaryType S>
    class Octree : public Tree<OctreeNode<S>> {
    private:

        int _maxDepth = 64;
        int _maxLeafSize = 32;

    public:

        using typename Tree<OctreeNode<S>>::Node;

        using Tree<Node>::Tree;
        using Tree<Node>::simulation;
        using Tree<Node>::root;

    protected:

        using Tree<Node>::depthSplit;
        using Tree<Node>::summarizeTreeTop;

    public:

        void build() override {

            BoundingBox boundingBox = outerBoundingBox<typename Node::Summary>(simulation());
            glm::vec3 dimensions = boundingBox.dimensions();
            root().center() = boundingBox.min() + (boundingBox.diagonal() / 2.0f);
            root().sideLength() = boundingBox.maxSideLength() * 1.1f;

            const auto &context = Node::Summary::context(simulation());
            int preBuildDepth = 2;
            auto toBeRefined = depthSplit(preBuildDepth, context);
            tbb::parallel_for_each(toBeRefined, [&](auto node) {
                node.get().refine(
                        _maxDepth,
                        [&](const auto &n) {
                            return n.contents().size() > _maxLeafSize;
                        },
                        context
                );
            });
            summarizeTreeTop(toBeRefined, context);
        };

        int &maxDepth() { return _maxDepth; }

        [[nodiscard]] const int &maxDepth() const { return _maxDepth; }

        int &maxLeafSize() { return _maxLeafSize; }

        [[nodiscard]] const int &maxLeafSize() const { return _maxLeafSize; }
    };

    template<std::size_t Order>
    using MultipoleActiveOctree = Octree<MultipoleMassSummary<Order>>;

    using ActiveOctree = Octree<CenterOfMassSummary>;
    using QuadrupoleActiveOctree = MultipoleActiveOctree<2>;
    using OctupoleActiveOctree = MultipoleActiveOctree<3>;

    template<std::size_t Order>
    using MultipolePassiveOctree = Octree<MultipoleAccelerationSummary<Order>>;

    using PassiveOctree = Octree<AccelerationSummary>;
    using QuadrupolePassiveOctree = MultipolePassiveOctree<2>;
    using OctupolePassiveOctree = MultipolePassiveOctree<3>;

    template<std::size_t Order>
    using MultipoleDualOctree = Octree<MultipoleDualSummary<Order>>;

    using DualOctree = Octree<DualSummary>;
    using QuadrupoleDualOctree = MultipoleDualOctree<2>;
    using OctupoleDualOctree = MultipoleDualOctree<3>;

    template<std::size_t Order>
    using MultipoleImplicitPassiveOctree = Octree<EmptySummary<MultipoleAccelerationSummary<Order>>>;
    using ImplicitPassiveOctree = Octree<EmptySummary<AccelerationSummary>>;
    using QuadrupoleImplicitPassiveOctree = MultipoleImplicitPassiveOctree<2>;
}

#endif //N_BODY_OCTREE_H
