//
// Created by jackcamp on 5/22/19.
//

#ifndef N_BODY_PRESET_H
#define N_BODY_PRESET_H


#include "../model/PhysicsContext.h"


// Useful constants

const auto white = rgbaColor(0.5, 0.5, 0.5, 0.5);
const auto yellow = rgbaColor(0.5, 0.5, 0, 0.5);
const auto red = rgbaColor(0.5, 0, 0, 0.5);
const auto orange = rgbaColor(0.5, 0.5, 0, 0.5);
const auto green = rgbaColor(0, 0.5, 0, 0.5);
const auto blue = rgbaColor(0, 0, 0.5, 0.5);
const auto teal = rgbaColor(0, 0.5, 0.5, 0.5);
const auto grey = rgbaColor(.5, .5, .5, 1);

class Preset {

public:

    // Tools

    void cubicGrid(Position cornerPosition = Position(-100, -100, -200), Velocity velocity = glm::vec3(0.0f, 0.0f,
                                                                                                       -100.0f),
                   glm::vec3 size = glm::vec3(20, 20, 20), float spacing = 10.0f, float mass = 10000.0f);

// Example configurations

    void blender();

    void bigDemo();

    void threeBodyDemo();


    PhysicsContext *getPhysicsContext() const {
        return physicsContext;
    }

    const std::vector<Body *> &getBodies() const {
        return bodies;
    }

private:

    PhysicsContext *physicsContext = new PhysicsContext;

    std::vector<Body *> bodies;
};


#endif //N_BODY_PRESET_H
