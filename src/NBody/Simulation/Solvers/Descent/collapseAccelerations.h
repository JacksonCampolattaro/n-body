//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_COLLAPSEACCELERATIONS_H
#define N_BODY_COLLAPSEACCELERATIONS_H

#include <NBody/Physics/Acceleration.h>
#include <NBody/Simulation/Simulation.h>

#include <bitset>

namespace NBody::Descent {

    using Physics::Acceleration;
    using Physics::MultipoleAcceleration;

    template<typename PassiveNode>
    requires requires(PassiveNode &p) {
        // fixme: this is a terrible way of constraining the template
        p.summary().acceleration().x;
    }
    inline void collapseAccelerations(PassiveNode &node,
                                      PassiveView &context,
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
    inline void collapseAccelerations(const PassiveNode &node,
                                      const PassiveView &context,
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
                                      const PassiveView &context) {

        using AccelerationType = typename std::remove_reference<decltype(node.summary().acceleration())>::type;
        collapseAccelerations(node, context, AccelerationType{});
    }


    // todo: this helper method could probably be made available elsewhere
    template<typename Node>
    inline const Node &childContaining(const Node &node, const Position &p) {
        assert(!node.isLeaf());
        auto relativeAddress = glm::lessThan((glm::vec3)node.center(), p);
        std::bitset<3> index{};
        index[2] = relativeAddress.x;
        index[1] = relativeAddress.y;
        index[0] = relativeAddress.z;
        return node.children()[index.to_ulong()];
    }

    template<typename PassiveNode>
    inline Acceleration accelerationAt(const PassiveNode &node, const Position &p) {
        if (node.isLeaf())
            return node.summary().acceleration().at(p - node.center());
        else
            return node.summary().acceleration().at(p - node.center()) + accelerationAt(childContaining(node, p), p);
    }
}

#endif //N_BODY_COLLAPSEACCELERATIONS_H
