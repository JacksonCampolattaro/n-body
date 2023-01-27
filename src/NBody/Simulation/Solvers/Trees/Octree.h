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

#include "Tree.h"
#include "ActiveNode.h"

#include "NBody/Simulation/Simulation.h"

#include "NBody/Physics/Rule.h"

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::Mass;
using NBody::Physics::ActiveTag;

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

    template<typename OctreeNodeImplementation>
    class OctreeNodeBase : public NodeBase<OctreeNodeImplementation> {
    private:

        std::vector<OctreeNodeImplementation> _children;

        Position _center{0.0f, 0.0f, 0.0f};
        float _sideLength{1.0f};

    public:

        using NodeBase<OctreeNodeImplementation>::NodeBase;
        using NodeBase<OctreeNodeImplementation>::contents;
        using NodeBase<OctreeNodeImplementation>::isLeaf;

        template<typename ViewType>
        void summarize(const ViewType &_) {}

        OctreeNodeBase(std::span<Entity> contents, const Position &center, float sideLength) :
                NodeBase<OctreeNodeImplementation>(contents),
                _center(center), _sideLength(sideLength) {}

        [[nodiscard]] std::vector<OctreeNodeImplementation> &children() { return _children; }

        [[nodiscard]] const std::vector<OctreeNodeImplementation> &children() const { return _children; }

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

        template<typename PositionViewType>
        void split(const PositionViewType &positions) {

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
            ) = partition<2>(contents(), positions, _center);

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

    template<typename OctreeNodeImplementation>
    class OctreeBase : public TreeBase<OctreeNodeImplementation> {
    private:

        int _maxDepth = 16;
        int _maxLeafSize = 64;

    public:

        OctreeBase(Simulation &simulation) : TreeBase<OctreeNodeImplementation>(simulation) {}

        using TreeBase<OctreeNodeImplementation>::simulation;
        using TreeBase<OctreeNodeImplementation>::root;

        void build() override {

            BoundingBox boundingBox = simulation().boundingBox();
            glm::vec3 dimensions = boundingBox.dimensions();
            root().center() = (boundingBox.max() - boundingBox.min()) / 2.0f;
            root().sideLength() = std::max(std::max(dimensions.x, dimensions.y), dimensions.z);

            const auto &context = OctreeNodeImplementation::constructionContext(simulation());
            int preBuildDepth = 2;
            auto toBeRefined = depthSplit(*this, preBuildDepth, context);
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

    class OctreeNode : public ActiveNode<OctreeNodeBase<OctreeNode>> {
    public:
        using ActiveNode::ActiveNode;
        using OctreeNodeBase::boundingBox;
    };

    class Octree : public OctreeBase<OctreeNode> {
    public:
        using OctreeBase::OctreeBase;
    };

}

#endif //N_BODY_OCTREE_H
