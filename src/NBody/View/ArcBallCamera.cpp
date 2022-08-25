//
// Created by jackcamp on 8/16/21.
//

#include "ArcBallCamera.h"

void NBody::ArcBallCamera::draw(const NBody::Simulation &simulation, const Vector2i &windowDimensions) {
    std::scoped_lock l(simulation.mutex);

    _renderer.draw(
            viewMatrix(),
            perspectiveProjection(Vector2{windowDimensions}.aspectRatio()),
            simulation
    );
}

