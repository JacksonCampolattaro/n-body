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

#include <NBody/Simulation/Solvers/Trees/NodeBase.h>

namespace NBody {

    using namespace Physics;

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
                _indices{relevantEntities<typename NodeImplementation::SummaryType>(_simulation)},
                _root{std::span<Entity>{_indices}} {

            // If the simulation has new particles, add them to the list
            _simulation.signal_particles_added.connect([&](auto newEntities) {
                _indices = relevantEntities<typename NodeImplementation::SummaryType>(_simulation);
                _root = NodeImplementation{_indices};
            });

            // If the simulation has particles removed, take them from the list
            _simulation.signal_particles_removed.connect([&](auto removedEntities) {
                _indices = relevantEntities<typename NodeImplementation::SummaryType>(_simulation);
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

    template<typename NodeType, typename Context>
    static std::vector<std::reference_wrapper<NodeType>> depthSplit(NodeType &root,
                                                                    std::size_t depth,
                                                                    Context &&context) {

        std::vector<std::reference_wrapper<NodeType>> queue;
        queue.push_back(root);

        std::vector<std::reference_wrapper<NodeType>> children;

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

    template<typename NodeType, typename Context>
    void summarizeTreeTop(NodeType &toBeSummarized,
                          const std::vector<std::reference_wrapper<NodeType>> &alreadySummarized,
                          Context &&context) {

        // If the node to be summarized is already in the summarized list, we don't need to summarize it again
        for (auto summarizedNode: alreadySummarized)
            if (&summarizedNode.get() == &toBeSummarized) return;

        for (auto &child: toBeSummarized.children()) {
            summarizeTreeTop(child, alreadySummarized, context);
        }

        toBeSummarized.summarize(context);
    }
}

#endif //T_SNE_TREE_H
