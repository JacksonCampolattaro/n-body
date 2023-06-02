//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_COLLAPSEACCELERATIONS_H
#define N_BODY_COLLAPSEACCELERATIONS_H

#include <NBody/Physics/Acceleration.h>
#include <NBody/Simulation/Simulation.h>

namespace NBody::Descent {

    using Physics::Acceleration;
    using Physics::MultipoleAcceleration;

    template<typename PassiveNode>
    requires requires(PassiveNode &p) {
        // fixme: this is a terrible way of constraining the template
        p.summary().acceleration().x;
    }
    inline void collapseAccelerations(PassiveNode &node,
                                      const entt::basic_view<
                                              Entity, entt::exclude_t<>,
                                              const Position,
                                              Acceleration
                                      > &context,
                                      Acceleration netAcceleration = {}) {

        netAcceleration += node.summary().acceleration();

        if (node.isLeaf()) {

            // When we reach a leaf node, apply the local force to all contained points
            for (auto i: node.contents())
                context.get<Acceleration>(i) += netAcceleration;

        } else {

            // Descend the tree recursively, keeping track of the net acceleration over the current region
            for (auto &child: node.children())
                collapseAccelerations(child, context, netAcceleration);

        }
    }

    template<typename PassiveNode, std::size_t Order>
    requires requires(PassiveNode &p) {
        p.summary().acceleration().vector();
    }
    inline void collapseAccelerations(PassiveNode &node,
                                      const entt::basic_view<
                                              Entity, entt::exclude_t<>,
                                              const Position,
                                              Acceleration
                                      > &context,
                                      MultipoleAcceleration<Order> netAcceleration = {}) {

        // Add the local multipole acceleration of this node to the accumulated acceleration in this location
        netAcceleration += node.summary().acceleration();

        if (node.isLeaf()) {

            // When we reach a leaf node, apply the local force to all contained points
            for (auto i: node.contents()) {

                // Find the vector from the center of this node to the particle's position
                // and re-center the multipole expansion on the particle
                auto localAcceleration = netAcceleration.at(context.get<const Position>(i) - node.center());

                // Apply the xyz component of the multipole acceleration to the particle
                context.get<Acceleration>(i) += localAcceleration;
            }

        } else {

            // Descend the tree recursively, keeping track of the net acceleration over the current region
            for (auto &child: node.children()) {

                // Find the vector from the center of this node to the center of the child
                // and re-center the multipole expansion on the child node's center
                auto localAcceleration = netAcceleration.translated(child.center() - node.center());

                // Apply the shifted expansion to the child node (recursive)
                collapseAccelerations(child, context, localAcceleration);
            }

        }
    }

    template<typename PassiveNode>
    requires requires(PassiveNode &p) {
        p.summary().acceleration().vector();
    }
    inline void collapseAccelerations(PassiveNode &node,
                                      const entt::basic_view<
                                              Entity, entt::exclude_t<>,
                                              const Position,
                                              Acceleration
                                      > &context) {

        using AccelerationType = typename std::remove_reference<decltype(node.summary().acceleration())>::type;
        collapseAccelerations(node, context, AccelerationType{});
    }
}

#endif //N_BODY_COLLAPSEACCELERATIONS_H
