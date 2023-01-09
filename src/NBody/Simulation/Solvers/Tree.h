//
// Created by Jackson Campolattaro on 12/11/22.
//

#ifndef T_SNE_TREE_H
#define T_SNE_TREE_H

#include <sigc++/sigc++.h>

#include <span>
#include <numeric>

#include <NBody/Physics/Rule.h>

#include "../Simulation.h"

namespace NBody {

    using namespace Physics;

    template<typename NodeImplementation>
    class NodeBase {
    private:

        std::span<Entity> _contents;
        Position _averagePosition{0.0f, 0.0f, 0.0f};

    public:

        NodeBase() : _contents() {}

        NodeBase(std::span<Entity> contents) : _contents(std::move(contents)) {}

        template<typename... Context>
        void refine(std::size_t maxDepth,
                    std::function<bool(const NodeImplementation &)> splitCriterion,
                    Context &&...context) {

            if (_contents.empty()) return;

            if (maxDepth > 0 && splitCriterion((const NodeImplementation &) *this)) {

                split(std::forward<Context>(context)...);

                for (auto &child: children())
                    child.refine(maxDepth - 1, splitCriterion, std::forward<Context>(context)...);

            } else {
                merge();
            }

            summarize(context...);
        }

        [[nodiscard]] const std::span<Entity> &contents() const { return _contents; };

        [[nodiscard]] std::span<Entity> &contents() { return _contents; };

        [[nodiscard]] const Position &averagePosition() const { return _averagePosition; }

        [[nodiscard]] std::size_t depth() const {
            if (isLeaf()) return 0;
            std::size_t maxDepth = 0;
            for (const NodeImplementation &child: children())
                maxDepth = std::max(maxDepth, child.depth());
            return maxDepth + 1;
        }

        [[nodiscard]] bool isLeaf() const { return children().empty(); }

        [[nodiscard]] auto &children() { return implementation().children(); };

        [[nodiscard]] const auto &children() const { return implementation().children(); };

        [[nodiscard]] auto &boundingBox() const { return implementation().boundingBox(); }

    protected:

        [[nodiscard]] Position &averagePosition() { return _averagePosition; }

        template<typename... Context>
        void split(Context &&...context) { implementation().split(std::forward<Context>(context)...); }

        template<typename... Context>
        void summarize(Context &&...context) { implementation().summarize(std::forward<Context>(context)...); }

        void merge() { implementation().merge(); }

    private:

        const NodeImplementation &implementation() const { return static_cast<const NodeImplementation &>(*this); }

        NodeImplementation &implementation() { return static_cast<NodeImplementation &>(*this); }

    };

    template<typename NodeImplementation>
    class TreeBase {
    public:

        using Node = NodeImplementation;

    private:

        Simulation &_simulation;
        std::vector<Entity> _indices;

        Node _root;

        Glib::Dispatcher _dispatcher;

    public:

        mutable sigc::signal<void()> signal_changed;
        mutable std::mutex mutex;

        TreeBase(Simulation &simulation) :
                _simulation(simulation),
                _indices{_simulation.group<const Position, const Mass>(entt::get<ActiveTag>).begin(),
                         _simulation.group<const Position, const Mass>(entt::get<ActiveTag>).end()},
                _root{std::span<Entity>{_indices}} {

            // todo: handle changes in the particle list
            // If the simulation has new particles, add them to the list
            _simulation.signal_particles_added.connect([&](auto newEntities) {
                auto actors = _simulation.group<const Position, const Mass>(entt::get<ActiveTag>);
                _indices = {actors.begin(), actors.end()};
                _root = NodeImplementation{_indices};
            });

            // If the simulation has particles removed, take them from the list
            _simulation.signal_particles_removed.connect([&](auto removedEntities) {
                auto actors = _simulation.group<const Position, const Mass>(entt::get<ActiveTag>);
                _indices = {actors.begin(), actors.end()};
                _root = NodeImplementation{_indices};
            });

            _dispatcher.connect(signal_changed.make_slot());
        }

        void refine() {
            std::scoped_lock lock{mutex};

            build();

            // Notify the UI of the change
            _dispatcher.emit();
        }

        virtual void build() = 0;

        [[nodiscard]] const Simulation &simulation() const { return _simulation; }

        [[nodiscard]] std::vector<Entity> &indices() { return _indices; }

        [[nodiscard]] const std::vector<Entity> &indices() const { return _indices; }

        [[nodiscard]] const NodeImplementation &root() const { return _root; }

        [[nodiscard]] NodeImplementation &root() { return _root; }

    };

    template<typename TreeNode>
    static inline Acceleration computeAcceleration(const TreeNode &node,
                                                   const entt::basic_view<
                                                           entt::entity, entt::exclude_t<>,
                                                           const Position, const Mass, const ActiveTag
                                                   > &activeParticles,
                                                   const Position &passivePosition,
                                                   const Mass &passiveMass,
                                                   const Rule &rule,
                                                   float theta) {

        // Empty nodes can be ignored
        if (node.contents().empty()) return Physics::Acceleration{};

        if ((node.sideLength() / glm::distance((glm::vec3) passivePosition, (glm::vec3) node.centerOfMass())) < theta) {

            // Node is treated as a single particle if S/D < theta (where S = sideLength and D = distance)
            return rule(node.centerOfMass(),
                        node.totalMass(),
                        passivePosition, passiveMass);

        } else {

            // Otherwise, the node can't be summarized
            if (node.isLeaf()) {

                // If this is a leaf node, interact with all particles contained
                return std::transform_reduce(
                        node.contents().begin(), node.contents().end(),
                        Physics::Acceleration{}, std::plus{},
                        [&](auto entity) {
                            return rule(activeParticles.get<const Position>(entity),
                                        activeParticles.get<const Mass>(entity),
                                        passivePosition, passiveMass);
                        }
                );

            } else {

                // If it's a non-leaf node, descend the tree (recursive case)
                return std::transform_reduce(
                        node.children().begin(), node.children().end(),
                        Physics::Acceleration{}, std::plus{},
                        [&](const auto &child) {
                            return computeAcceleration(
                                    child, activeParticles,
                                    passivePosition, passiveMass,
                                    rule, theta
                            );
                        }
                );
            }
        }
    }
}

#endif //T_SNE_TREE_H
