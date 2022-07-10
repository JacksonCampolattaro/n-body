//
// Created by Jackson Campolattaro on 7/3/22.
//

#include "Octree.h"

NBody::Octree::OctreeNode::OctreeNode(std::span<NBody::Entity> contents, const NBody::Simulation &registry,
                                      Position center, float sideLength)
        : _contents(contents),
          _center(center),
          _sideLength(sideLength) {

    // Subdivide if this node contains too many particles
    // todo We can probably get more performance by allowing leaf nodes to contain more than one particle
    if (contents.size() > 32 && sideLength > 0.1) {

        // todo This is an awful, ugly way of doing this; later it can be replaced with bitset indices

        // Split on x-axis
        auto [x0, x1] = split<0>(_contents, registry);

        // Split on y-axis
        auto [xy00, xy01] = split<1>(x0, registry);
        auto [xy10, xy11] = split<1>(x1, registry);

        // Split on z-axis
        auto [xyz000, xyz001] = split<2>(xy00, registry);
        auto [xyz010, xyz011] = split<2>(xy01, registry);
        auto [xyz100, xyz101] = split<2>(xy10, registry);
        auto [xyz110, xyz111] = split<2>(xy11, registry);

        // Initialize 8 child nodes
        float childSideLength = sideLength / 2.0f;
        _children = std::unique_ptr<std::array<OctreeNode, 8>>(new std::array<OctreeNode, 8>{
                {
                        {xyz000, registry,
                         center + glm::vec3{-childSideLength, -childSideLength, -childSideLength},
                         childSideLength},
                        {xyz001, registry,
                         center + glm::vec3{-childSideLength, -childSideLength, childSideLength},
                         childSideLength},
                        {xyz010, registry,
                         center + glm::vec3{-childSideLength, childSideLength, -childSideLength},
                         childSideLength},
                        {xyz011, registry,
                         center + glm::vec3{-childSideLength, childSideLength, childSideLength},
                         childSideLength},
                        {xyz100, registry,
                         center + glm::vec3{childSideLength, -childSideLength, -childSideLength},
                         childSideLength},
                        {xyz101, registry,
                         center + glm::vec3{childSideLength, -childSideLength, childSideLength},
                         childSideLength},
                        {xyz110, registry,
                         center + glm::vec3{childSideLength, childSideLength, -childSideLength},
                         childSideLength},
                        {xyz111, registry,
                         center + glm::vec3{childSideLength, childSideLength, childSideLength},
                         childSideLength}
                }
        });

    }

    // Summarize mass & position
    if (_children) {

        for (const auto &child: *_children) {
            _totalMass.mass() += child.totalMass().mass();
            _centerOfMass = _centerOfMass + (child.centerOfMass() * child.totalMass().mass());
        }
        _centerOfMass = _centerOfMass / _totalMass.mass();

    } else {

        for (const auto &entity: _contents) {
            auto entityMass = registry.get<ActiveMass>(entity).mass();
            _totalMass.mass() += entityMass;
            _centerOfMass = _centerOfMass + (entityMass * registry.get<Position>(entity));
        }
        _centerOfMass = _centerOfMass / _totalMass.mass();

    }

    for (const auto &entity: _contents) {
        assert(registry.valid(entity));
    }
}
