//
// Created by Jackson Campolattaro on 7/3/22.
//

#include "Octree.h"

NBody::Octree::OctreeNode::OctreeNode(std::span<NBody::Entity> contents, const NBody::Simulation &registry,
                                      Position center, float sideLength,
                                      int maxDepth, int maxLeafSize)
        : _contents(contents),
          _center(center),
          _sideLength(sideLength) {

    // Subdivide if this node contains too many particles
    // todo We can probably get more performance by allowing leaf nodes to contain more than one particle
    if (contents.size() > maxLeafSize && sideLength > 0.1 && maxDepth > 0) {

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
        ) = split<2>(_contents, registry);

        // Initialize 8 child nodes
        float childSideLength = sideLength / 2.0f;
        _children = std::unique_ptr<std::array<OctreeNode, 8>>(new std::array<OctreeNode, 8>{
                {
                        {xyz000, registry,
                         center + glm::vec3{-childSideLength, -childSideLength, -childSideLength},
                         childSideLength, maxDepth - 1, maxLeafSize},
                        {xyz001, registry,
                         center + glm::vec3{-childSideLength, -childSideLength, childSideLength},
                         childSideLength, maxDepth - 1, maxLeafSize},
                        {xyz010, registry,
                         center + glm::vec3{-childSideLength, childSideLength, -childSideLength},
                         childSideLength, maxDepth - 1, maxLeafSize},
                        {xyz011, registry,
                         center + glm::vec3{-childSideLength, childSideLength, childSideLength},
                         childSideLength, maxDepth - 1, maxLeafSize},
                        {xyz100, registry,
                         center + glm::vec3{childSideLength, -childSideLength, -childSideLength},
                         childSideLength, maxDepth - 1, maxLeafSize},
                        {xyz101, registry,
                         center + glm::vec3{childSideLength, -childSideLength, childSideLength},
                         childSideLength, maxDepth - 1, maxLeafSize},
                        {xyz110, registry,
                         center + glm::vec3{childSideLength, childSideLength, -childSideLength},
                         childSideLength, maxDepth - 1, maxLeafSize},
                        {xyz111, registry,
                         center + glm::vec3{childSideLength, childSideLength, childSideLength},
                         childSideLength, maxDepth - 1, maxLeafSize}
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
