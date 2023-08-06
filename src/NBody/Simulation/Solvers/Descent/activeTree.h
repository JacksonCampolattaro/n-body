//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_ACTIVETREE_H
#define N_BODY_ACTIVETREE_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>

#include <tbb/concurrent_queue.h>
#include <tbb/parallel_pipeline.h>
#include <tbb/flow_graph.h>

#include <boost/container/small_vector.hpp>
#include <boost/container/static_vector.hpp>

#include <semaphore>

#include "none.h"

namespace NBody::Descent {

    template<NodeType ActiveNode, DescentCriterionType DescentCriterion, RuleType Rule>
    inline Acceleration activeTree(
            const ActiveNode &node,
            const Position &passivePosition,
            const DescentCriterion &descentCriterion,
            Rule &rule,
            const ActiveView &context
    ) {

        // Empty nodes can be ignored
        if (node.contents().empty()) return Physics::Acceleration{};

        if (descentCriterion(node, passivePosition)) {
            return rule(node, passivePosition);
        } else {

            // Otherwise, the node can't be summarized
            if (node.isLeaf()) {

                // If this is a leaf node, interact with all particles contained
                Physics::Acceleration acceleration{};
                #pragma ivdep
                for (std::size_t i = 0; i < node.contents().size(); ++i) {
                    auto entity = node.contents()[i];
                    acceleration += rule(context.get<const Position>(entity),
                                         context.get<const Mass>(entity),
                                         passivePosition);
                }
                return acceleration;
                //                return std::transform_reduce(
                //                        node.contents().begin(), node.contents().end(),
                //                        Physics::Acceleration{}, std::plus{},
                //                        [&](auto entity) {
                //                            return rule(context.get<const Position>(entity),
                //                                        context.get<const Mass>(entity),
                //                                        passivePosition);
                //                        }
                //                );

            } else {

                // If it's a non-leaf node, descend the tree (recursive case)
                return std::transform_reduce(
                        node.children().begin(), node.children().end(),
                        Physics::Acceleration{}, std::plus{},
                        [&](const auto &child) {
                            return activeTree(
                                    child, passivePosition,
                                    descentCriterion, rule,
                                    context
                            );
                        }
                );
            }
        }
    }

    template<NodeType ActiveNode, DescentCriterionType DescentCriterion, RuleType Rule>
    inline Acceleration activeTreeFarNodesFirst(
            const ActiveNode &node,
            const Position &passivePosition,
            const DescentCriterion &descentCriterion,
            Rule &rule,
            const ActiveView &context
    ) {
        assert(!node.isLeaf());

        // Treat nodes which can be handled immediately first, sort out the rest
        Acceleration netAcceleration{};
        boost::container::static_vector<std::reference_wrapper<const ActiveNode>, 8> nearNodes, leafNodes;
        for (const ActiveNode &child: node.children()) {
            if (child.contents().empty())
                continue;
            else if (descentCriterion(child, passivePosition))
                netAcceleration += rule(child, passivePosition);
            else if (child.isLeaf())
                leafNodes.emplace_back(child);
            else
                nearNodes.emplace_back(child);
        }

        // Handle leaf nodes (requires retrieving particle data)
        for (const ActiveNode &child: leafNodes)
            netAcceleration += std::transform_reduce(
                    child.contents().begin(), child.contents().end(),
                    Physics::Acceleration{}, std::plus{},
                    [&](auto entity) {
                        return rule(context.get<const Position>(entity),
                                    context.get<const Mass>(entity),
                                    passivePosition);
                    }
            );

        // Handle nearby nodes (requires retrieving child data)
        for (const ActiveNode &child: nearNodes)
            netAcceleration += std::transform_reduce(
                    child.children().begin(), child.children().end(),
                    Physics::Acceleration{}, std::plus{},
                    [&](const auto &child) {
                        return activeTree(
                                child, passivePosition,
                                descentCriterion, rule,
                                context
                        );
                    }
            );

        return netAcceleration;

    }

    // fixme: There's a race condition somewhere!
    // I also think this could perform better if atomics were avoided
    template<NodeType ActiveNode, DescentCriterionType DescentCriterion, RuleType Rule>
    inline Acceleration pipelineActiveTree(
            const ActiveNode &node,
            const Position &passivePosition,
            const DescentCriterion &descentCriterion,
            Rule &rule,
            const ActiveView &context
    ) {

        // Process a queue of nodes, starting with the one passed as an argument
        // fixme: parallel_pipeline doesn't support const pointers!
        std::atomic<std::size_t> nodesUnderEvaluation = 0;
        tbb::concurrent_queue<ActiveNode *> nodeQueue{};
        nodeQueue.emplace(const_cast<ActiveNode *>(&node));

        // Compute interactions with a queue of node summaries and a queue of particles
        enum class InteractionCase {
            NONE,
            INTERACTION_WITH_PARTICLES,
            INTERACTION_WITH_NODE,
        };
        tbb::concurrent_queue<typename ActiveNode::Summary> summaryQueue{};
        tbb::concurrent_queue<std::pair<Position, Mass>> particleQueue{};

        // Resulting forces will be accumulated at the end of the pipeline
        Acceleration netAcceleration{};

        // For each node in the nodeQueue
        tbb::parallel_pipeline(

                // How do I choose the # of live tokens?
                8,

                // Pull nodes from the nodeQueue
                // Maybe prefetch relevant data for the descent criterion
                // (serial)
                make_filter<
                        void,
                        ActiveNode *
                >(
                        tbb::filter_mode::serial_out_of_order,
                        //tbb::filter_mode::serial_in_order,
                        [&](tbb::flow_control &fc) -> ActiveNode * {

                            if (nodeQueue.empty() && nodesUnderEvaluation == 0) {
                                fc.stop();
                                return nullptr;
                            }

                            ActiveNode *n;
                            nodesUnderEvaluation++;
                            if (!nodeQueue.try_pop(n) || n->contents().empty()) {
                                nodesUnderEvaluation--;
                                return nullptr;
                            }
                            // todo Maybe I can use __builtin_prefetch() to get node data ahead of time?
                            return n;
                        }
                ) &

                // Check if nodes pass our descent criterion,
                // annotate them with the result
                // (parallel)
                make_filter<
                        ActiveNode *,
                        std::pair<ActiveNode *, InteractionCase>
                >(
                        tbb::filter_mode::parallel,
                        //tbb::filter_mode::serial_in_order,
                        [&](ActiveNode *n) -> std::pair<ActiveNode *, InteractionCase> {

                            if (!n) return {nullptr, InteractionCase::NONE};

                            if (descentCriterion(*n, passivePosition))
                                return {n, InteractionCase::INTERACTION_WITH_NODE};
                            else if (n->isLeaf())
                                return {n, InteractionCase::INTERACTION_WITH_PARTICLES};
                            else
                                return {n, InteractionCase::NONE};
                        }
                ) &

                // Push to the relevant queues
                // Nodes which passed the descent criterion have their info added to the summary queue
                // Those that fail have all their children added to the node queue
                // Unless they are leaves, in which case all their particles are added to the particle queue
                // Return an enum indicating which queue was updated
                // (serial)
                make_filter<
                        std::pair<ActiveNode *, InteractionCase>,
                        InteractionCase
                >(
                        tbb::filter_mode::serial_out_of_order,
                        //tbb::filter_mode::serial_in_order,
                        [&](std::pair<ActiveNode *, InteractionCase> annotatedNode) -> InteractionCase {

                            auto &[n, interactionCase] = annotatedNode;
                            if (!n) return InteractionCase::NONE;

                            if (interactionCase == InteractionCase::NONE) {
                                for (auto &c: n->children())
                                    nodeQueue.emplace(&c);
                            }

                            if (interactionCase == InteractionCase::INTERACTION_WITH_NODE)
                                summaryQueue.push(n->summary());

                            if (interactionCase == InteractionCase::INTERACTION_WITH_PARTICLES) {
                                for (auto e: n->contents()) {
                                    particleQueue.emplace(
                                            context.get<Position>(e),
                                            context.get<Mass>(e)
                                    );
                                }
                            }

                            nodesUnderEvaluation--;
                            return interactionCase;
                        }
                ) &

                // Handle interactions, processing whichever queue was just updated
                // Return the total acceleration produced by the interactions just computed
                // (parallel)
                make_filter<
                        InteractionCase,
                        Acceleration
                >(
                        tbb::filter_mode::parallel,
                        //tbb::filter_mode::serial_in_order,
                        [&](InteractionCase interactionCase) -> Acceleration {

                            Acceleration acceleration{};

                            typename ActiveNode::Summary summary;
                            while (summaryQueue.try_pop(summary))
                                acceleration += rule(summary, passivePosition);

                            std::pair<Position, Mass> activeParticle;
                            while (particleQueue.try_pop(activeParticle))
                                acceleration += rule(activeParticle.first, activeParticle.second, passivePosition);

                            return acceleration;
                        }
                ) &

                // Add up the net acceleration
                // (sequential)
                make_filter<
                        Acceleration,
                        void
                >(
                        tbb::filter_mode::serial_out_of_order,
                        //tbb::filter_mode::serial_in_order,
                        [&](Acceleration acceleration) -> void {
                            netAcceleration += acceleration;
                        }
                )

        );

        return netAcceleration;

    }

    template<NodeType ActiveNode, DescentCriterionType DescentCriterion, RuleType Rule>
    inline Acceleration queueActiveTree(
            const ActiveNode &node,
            const Position &passivePosition,
            const DescentCriterion &descentCriterion,
            Rule &rule,
            const ActiveView &context
    ) {

        Acceleration netAcceleration{};

        std::queue<std::span<const ActiveNode>> queue;
        std::vector<std::span<Entity>> leafEntities;

        if (node.isLeaf())
            leafEntities.emplace_back(node.contents());
        else
            queue.emplace(node.children());

        std::size_t maxLength = 0;
        while (!queue.empty()) {
            maxLength = std::max(maxLength, queue.size());
            auto children = queue.front();
            queue.pop();

            for (const ActiveNode &child: children) {
                if (descentCriterion(child, passivePosition)) {
                    netAcceleration += rule(child, passivePosition);
                } else if (child.isLeaf()) {
                    leafEntities.emplace_back(child.contents());
                } else {
                    queue.emplace(child.children());
                }
            }
        }

//        // Combine all the entities into one list
//        std::vector<Entity> mergedLeafEntities;
//        for (const auto &entitiesGroup: leafEntities) {
//            for (auto e: entitiesGroup) {
//                mergedLeafEntities.emplace_back(e);
//            }
//        }
//        netAcceleration += std::transform_reduce(
//                mergedLeafEntities.begin(), mergedLeafEntities.end(),
//                Physics::Acceleration{}, std::plus{},
//                [&](auto entity) {
//                    return rule(context.get<const Position>(entity),
//                                context.get<const Mass>(entity),
//                                passivePosition);
//                }
//        );

        for (const auto &group: leafEntities)
            netAcceleration += std::transform_reduce(
                    group.begin(), group.end(),
                    Physics::Acceleration{}, std::plus{},
                    [&](auto entity) {
                        return rule(context.get<const Position>(entity),
                                    context.get<const Mass>(entity),
                                    passivePosition);
                    }
            );

        //spdlog::error("{}, {}", maxLength, leaves.size());
        return netAcceleration;
    }

}

#endif //N_BODY_ACTIVETREE_H
