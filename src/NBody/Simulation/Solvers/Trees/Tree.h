//
// Created by Jackson Campolattaro on 12/11/22.
//

#ifndef T_SNE_TREE_H
#define T_SNE_TREE_H

#include <sigc++/sigc++.h>

#include <span>
#include <numeric>

#include "NBody/Physics/Rule.h"

#include "NBody/Simulation/Simulation.h"

namespace NBody {

    using namespace Physics;

    template<typename NodeImplementation>
    class NodeBase {
    private:

        std::span<Entity> _contents;

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
                _indices{NodeImplementation::relevantEntities(_simulation)},
                _root{std::span<Entity>{_indices}} {

            // If the simulation has new particles, add them to the list
            _simulation.signal_particles_added.connect([&](auto newEntities) {
                _indices = NodeImplementation::relevantEntities(_simulation);
                _root = NodeImplementation{_indices};
            });

            // If the simulation has particles removed, take them from the list
            _simulation.signal_particles_removed.connect([&](auto removedEntities) {
                _indices = NodeImplementation::relevantEntities(_simulation);
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

        [[nodiscard]]  Simulation &simulation() { return _simulation; }

        [[nodiscard]] std::vector<Entity> &indices() { return _indices; }

        [[nodiscard]] const std::vector<Entity> &indices() const { return _indices; }

        [[nodiscard]] const NodeImplementation &root() const { return _root; }

        [[nodiscard]] NodeImplementation &root() { return _root; }

    };

    template<typename TreeType>
    static std::vector<std::reference_wrapper<typename TreeType::Node>> loadBalancedSplit(TreeType &tree,
                                                                                          std::size_t n) {

        std::vector<std::reference_wrapper<typename TreeType::Node>> queue;
        auto comparator = [&](std::reference_wrapper<typename TreeType::Node> a,
                              std::reference_wrapper<typename TreeType::Node> b) {
            return a.get().contents().size() < b.get().contents().size();
        };

        queue.push_back(tree.root());

        while (queue.size() < n) {

            std::pop_heap(queue.begin(), queue.end(), comparator);
            typename TreeType::Node &toSplit = queue.back();
            if (toSplit.isLeaf()) break;
            queue.pop_back();

            for (typename TreeType::Node &child: toSplit.children()) {
                queue.push_back(child);
                std::push_heap(queue.begin(), queue.end(), comparator);
            }
        }

        return queue;
    }

    template<typename TreeType, typename ...Context>
    static std::vector<std::reference_wrapper<typename TreeType::Node>> depthSplit(TreeType &tree,
                                                                                   std::size_t depth,
                                                                                   Context&&... context) {

        std::vector<std::reference_wrapper<typename TreeType::Node>> queue;
        queue.push_back(tree.root());

        std::vector<std::reference_wrapper<typename TreeType::Node>> children;

        for (int i = 0; i < depth; ++i) {
            for (auto &node: queue) {
                node.get().split(context...);

                for (auto &child: node.get().children()) {
                    children.push_back(child);
                }
            }

            queue = children;
            children.clear();
        }

        return queue;
    }

    template<typename TreeNode, typename ...Context>
    void summarizeTreeTop(TreeNode &toBeSummarized,
                          const std::vector<std::reference_wrapper<TreeNode>> &alreadySummarized,
                          Context&&... context) {

        // If the node to be summarized is already in the summarized list, we don't need to summarize it again
        for (auto summarizedNode : alreadySummarized)
            if (&summarizedNode.get() == &toBeSummarized) return;

        for (auto &child: toBeSummarized.children()) {
            summarizeTreeTop(child, alreadySummarized, context...);
        }

        toBeSummarized.summarize(context...);
    }
}

#endif //T_SNE_TREE_H