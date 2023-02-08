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
#include <NBody/Simulation/Solvers/Trees/Summaries/CenterOfMassSummary.h>
#include <NBody/Simulation/Solvers/Trees/Summaries/AccelerationSummary.h>
#include <NBody/Simulation/Solvers/Trees/Summaries/DualSummary.h>

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

        if constexpr(Dimension == 0) {

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

    template<Summary SummaryType>
    class OctreeNode : public NodeBase<OctreeNode<SummaryType>, SummaryType> {
    private:

        std::vector<OctreeNode> _children;

        Position _center{0.0f, 0.0f, 0.0f};
        float _sideLength{1.0f};

    public:

        using NodeBase<OctreeNode, SummaryType>::NodeBase;
        using NodeBase<OctreeNode, SummaryType>::contents;
        using NodeBase<OctreeNode, SummaryType>::isLeaf;

        template<typename Context>
        void summarize(const Context &context) {
            NodeBase<OctreeNode, SummaryType>::summarize(context);
        }

        OctreeNode(std::span<Entity> contents, const Position &center, float sideLength) :
                NodeBase<OctreeNode, SummaryType>(contents),
                _center(center), _sideLength(sideLength) {}

        [[nodiscard]] std::vector<OctreeNode> &children() { return _children; }

        [[nodiscard]] const std::vector<OctreeNode> &children() const { return _children; }

        [[nodiscard]] BoundingBox boundingBox() const {
            glm::vec3 dimensions{_sideLength, _sideLength, _sideLength};
            return {
                    {_center - dimensions},
                    {_center + dimensions}
            };
        }

        [[nodiscard]] Position &center() { return _center; }

        [[nodiscard]] const Position &center() const { return _center; }

        [[nodiscard]] const float &sideLength() const { return _sideLength; }

        [[nodiscard]] float &sideLength() { return _sideLength; }

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
            _children = {{
                                 {xyz000,
                                  center() + glm::vec3{-childSideLength, -childSideLength, -childSideLength},
                                  childSideLength},
                                 {xyz001,
                                  center() + glm::vec3{-childSideLength, -childSideLength, childSideLength},
                                  childSideLength},
                                 {xyz010,
                                  center() + glm::vec3{-childSideLength, childSideLength, -childSideLength},
                                  childSideLength},
                                 {xyz011,
                                  center() + glm::vec3{-childSideLength, childSideLength, childSideLength},
                                  childSideLength},
                                 {xyz100,
                                  center() + glm::vec3{childSideLength, -childSideLength, -childSideLength},
                                  childSideLength},
                                 {xyz101,
                                  center() + glm::vec3{childSideLength, -childSideLength, childSideLength},
                                  childSideLength},
                                 {xyz110,
                                  center() + glm::vec3{childSideLength, childSideLength, -childSideLength},
                                  childSideLength},
                                 {xyz111,
                                  center() + glm::vec3{childSideLength, childSideLength, childSideLength},
                                  childSideLength}
                         }};
        }

        void merge() {
            _children.clear();
        }

    };

    template<Summary S>
    class Octree : public Tree<OctreeNode<S>> {
    private:

        int _maxDepth = 16;
        int _maxLeafSize = 64;

    public:

        using typename Tree<OctreeNode<S>>::Node;

        using Tree<Node>::Tree;
        using Tree<Node>::simulation;
        using Tree<Node>::root;

        void build() override {

            BoundingBox boundingBox = outerBoundingBox<typename Node::SummaryType>(simulation());
            glm::vec3 dimensions = boundingBox.dimensions();
            root().center() = (boundingBox.max() - boundingBox.min()) / 2.0f;
            root().sideLength() = std::max(std::max(dimensions.x, dimensions.y), dimensions.z);

            const auto &context = Node::SummaryType::context(simulation());
            int preBuildDepth = 2;
            auto toBeRefined = depthSplit(root(), preBuildDepth, context);
            tbb::parallel_for_each(toBeRefined, [&](auto node) {
                node.get().refine(
                        _maxDepth,
                        [&](const auto &n) {
                            return n.contents().size() > _maxLeafSize;
                        },
                        context
                );
            });
            summarizeTreeTop(root(), toBeRefined, context);

            // todo: maybe non-parallel construction should be available as an option?
            //            root().refine(
            //                    _maxDepth,
            //                    [&](const auto &n) { return n.contents().size() > _maxLeafSize; },
            //                    simulation().template view<const Position, const Mass, const ActiveTag>()
            //            );
        };

        int &maxDepth() { return _maxDepth; }

        const int &maxDepth() const { return _maxDepth; }

        int &maxLeafSize() { return _maxLeafSize; }

        const int &maxLeafSize() const { return _maxLeafSize; }
    };

    class ActiveOctree : public Octree<CenterOfMassSummary> {
    public:
        using Octree<CenterOfMassSummary>::Octree;
    };

    class PassiveOctree : public Octree<AccelerationSummary> {
    public:
        using Octree<AccelerationSummary>::Octree;
    };

    class DualOctree : public Octree<DualSummary> {
    public:
        using Octree<DualSummary>::Octree;
    };

}

#endif //N_BODY_OCTREE_H
