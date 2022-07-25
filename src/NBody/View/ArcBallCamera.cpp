//
// Created by jackcamp on 8/16/21.
//

#include "ArcBallCamera.h"

void NBody::ArcBallCamera::draw(const NBody::Simulation &simulation) {
    std::scoped_lock l(simulation.mutex);

    _renderer.draw(viewMatrix(), perspectiveProjection(), simulation);
}

