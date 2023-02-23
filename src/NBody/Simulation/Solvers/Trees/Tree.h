//
// Created by Jackson Campolattaro on 12/11/22.
//

#ifndef T_SNE_TREE_H
#define T_SNE_TREE_H

#include <sigc++/sigc++.h>

#include <span>
#include <numeric>

#include <NBody/Physics/Rule.h>
#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/Trees/NodeType.h>

namespace NBody {

    using namespace Physics;

    template<NodeType N>
    class Tree {
    public:

        using Node = N;

    private:

        Simulation &_simulation;
        std::vector<Entity> _indices;

        Node _root;

        Glib::Dispatcher _dispatcher;

    public:

        mutable sigc::signal<void()> signal_changed;
        mutable std::mutex mutex;

        Tree(Simulation &simulation) :
                _simulation(simulation),
                _indices{relevantEntities<typename Node::Summary>(_simulation)},
                _root{std::span<Entity>{_indices}} {

            // If the simulation has new particles, add them to the list
            _simulation.signal_particles_added.connect([&](auto newEntities) {
                _indices = relevantEntities<typename Node::Summary>(_simulation);
                _root = Node{_indices};
            });

            // If the simulation has particles removed, take them from the list
            _simulation.signal_particles_removed.connect([&](auto removedEntities) {
                _indices = relevantEntities<typename Node::Summary>(_simulation);
                _root = Node{_indices};
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

        [[nodiscard]] const Node &root() const { return _root; }

        [[nodiscard]] Node &root() { return _root; }

        [[nodiscard]] std::vector<std::reference_wrapper<Node>> loadBalancedBreak(std::size_t n) {

            std::vector<std::reference_wrapper<Node>> queue;
            auto comparator = [&](std::reference_wrapper<Node> a, std::reference_wrapper<Node> b) {
                return a.get().contents().size() < b.get().contents().size();
            };

            queue.push_back(root());

            while (queue.size() < n) {

                std::pop_heap(queue.begin(), queue.end(), comparator);
                Node &toBreak = queue.back();
                if (toBreak.isLeaf()) break;
                queue.pop_back();

                for (Node &child: toBreak.children()) {
                    queue.push_back(child);
                    std::push_heap(queue.begin(), queue.end(), comparator);
                }
            }

            return queue;
        }

        [[nodiscard]] std::vector<std::reference_wrapper<Node>> depthBreak(std::size_t depth) {

            std::vector<std::reference_wrapper<Node>> queue, children;
            queue.push_back(root());

            for (int i = 0; i < depth; ++i) {
                for (auto &node: queue) {

                    // if we bump into a leaf node, we should stop descending early
                    if (node.get().isLeaf()) spdlog::error("?");

                    for (auto &child: node.get().children()) {
                        children.push_back(child);
                    }
                }

                queue = children;
                children.clear();
            }

            return queue;
        }


    protected:

        template<typename Context>
        [[nodiscard]] std::vector<std::reference_wrapper<Node>> depthSplit(std::size_t depth, Context &&context) {

            std::vector<std::reference_wrapper<Node>> queue;
            queue.push_back(root());

            std::vector<std::reference_wrapper<Node>> children;

            for (int i = 0; i < depth; ++i) {
                for (auto &node: queue) {
                    node.get().split(context);

                    for (auto &child: node.get().children()) {
                        children.push_back(child);
                    }
                }

                queue = children;
                children.clear();
            }

            return queue;
        }

        template<typename Context>
        void summarizeTreeTop(const std::vector<std::reference_wrapper<Node>> &alreadySummarized,
                              Context &&context) {
            summarizeTreeTop(root(), alreadySummarized, context);
        }

        template<typename Context>
        void summarizeTreeTop(Node &toBeSummarized,
                              const std::vector<std::reference_wrapper<Node>> &alreadySummarized,
                              Context &&context) {

            // If the node to be summarized is already in the summarized list, we don't need to summarize it again
            for (auto &summarizedNode: alreadySummarized)
                if (&summarizedNode.get() == &toBeSummarized) return;

            for (auto &child: toBeSummarized.children()) {
                summarizeTreeTop(child, alreadySummarized, context);
            }

            toBeSummarized.summarize(context);
        }

    };

}

#endif //T_SNE_TREE_H
