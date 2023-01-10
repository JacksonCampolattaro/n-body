//
// Created by Jackson Campolattaro on 1/9/23.
//

#ifndef N_BODY_LINEARBVH_H
#define N_BODY_LINEARBVH_H

#include "Tree.h"

#include "NBody/Simulation/Solvers/MortonSort.h"

#include "NBody/Simulation/BoundingBox.h"

namespace NBody {

    class LinearBVHNode : public NodeBase<LinearBVHNode> {
    private:

        std::vector<LinearBVHNode> _children;

        BoundingBox _boundingBox{};
        Mass _totalMass = 0.0f;
        Position _centerOfMass = {0.0f, 0.0f, 0.0f};

    public:

        LinearBVHNode(std::span<Entity> contents) :
                NodeBase<LinearBVHNode>(contents) {}

        using NodeBase<LinearBVHNode>::contents;
        using NodeBase<LinearBVHNode>::isLeaf;

        [[nodiscard]] std::vector<LinearBVHNode> &children() { return _children; }

        [[nodiscard]] const std::vector<LinearBVHNode> &children() const { return _children; }

        [[nodiscard]] const BoundingBox &boundingBox() const { return _boundingBox; }

        [[nodiscard]] const Mass &totalMass() const { return _totalMass; }

        [[nodiscard]] const Position &centerOfMass() const { return _centerOfMass; }

        void split(const entt::basic_view<entt::entity, entt::exclude_t<>,
                const Position, const Mass, const ActiveTag, const MortonCode> &context) {

            // We need to determine where to split
            // Start by finding the most significant bit that changes between the start and the end of the coordinates
            // (because of our split criterion, we have a guarantee that they aren't the same)
            int msb = fls(context.get<const MortonCode>(contents().front()) ^
                          context.get<const MortonCode>(contents().back())) - 1;

            // Use std::upper_bound to perform a binary search and find where that bit changes
            MortonCode boundary = (1 << msb);
            auto split = std::upper_bound(contents().begin(), contents().end(), boundary,
                                          [&](MortonCode v, Entity i) {
                                              return v & context.get<const MortonCode>(i);
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

        void summarize(const entt::basic_view<entt::entity, entt::exclude_t<>,
                const Position, const Mass, const ActiveTag, const MortonCode> &context) {

            _boundingBox = BoundingBox{};
            _totalMass.mass() = 0.0f;
            _centerOfMass = {0.0f, 0.0f, 0.0f};

            if (isLeaf()) {

                for (const auto &entity: contents()) {

                    auto entityPosition = context.get<const Position>(entity);
                    auto entityMass = context.get<const Mass>(entity).mass();

                    _boundingBox.min() = glm::min((glm::vec3) entityPosition, (glm::vec3) _boundingBox.min());
                    _boundingBox.max() = glm::max((glm::vec3) entityPosition, (glm::vec3) _boundingBox.max());

                    _totalMass.mass() += entityMass;
                    _centerOfMass += (entityMass * entityPosition);
                }
                _centerOfMass = _centerOfMass / _totalMass.mass();

            } else {

                for (const auto &child: children()) {

                    _boundingBox.min() = glm::min((glm::vec3) child.boundingBox().min(),
                                                  (glm::vec3) _boundingBox.min());
                    _boundingBox.max() = glm::max((glm::vec3) child.boundingBox().max(),
                                                  (glm::vec3) _boundingBox.max());

                    _totalMass.mass() += child.totalMass().mass();
                    _centerOfMass += (child.centerOfMass() * child.totalMass().mass());
                }
                _centerOfMass = _centerOfMass / _totalMass.mass();

            }

            // Ensure the center of mass is inside the bounding box
            // (this may not be true because of rounding errors)
            _centerOfMass = glm::min(glm::max((glm::vec3) _boundingBox.min(), _centerOfMass), _boundingBox.max());

        }

        void merge() {
            _children.clear();
        }

    };

    class LinearBVH : public TreeBase<LinearBVHNode> {
    public:

        LinearBVH(Simulation &simulation) : TreeBase<LinearBVHNode>(simulation) {}

        void build() override {

            // Assign morton codes to all active particles
            setMortonCodes(simulation());

            // Sort the indices by the associated morton codes
            mortonSort(simulation().view<const MortonCode>(), indices());

            // Build the tree
            auto context = simulation().view<const Position, const Mass, const ActiveTag, const MortonCode>();
            root().refine(
                    std::numeric_limits<std::size_t>::max(),
                    [&](const auto &n) {
                        // Don't split if all entities in this node have the same morton code
                        return context.get<const MortonCode>(n.contents().front()) !=
                               context.get<const MortonCode>(n.contents().back());
                    },
                    context
            );
        }

    };

}

#endif //N_BODY_LINEARBVH_H
