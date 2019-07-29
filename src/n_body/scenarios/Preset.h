//
// Created by jackcamp on 5/22/19.
//

#ifndef N_BODY_PRESET_H
#define N_BODY_PRESET_H


#include "../view/View.h"
#include "../model/PhysicsContext.h"


// Useful constants

const glm::vec3 white = glm::vec3(1, 1, 1);
const glm::vec3 yellow = glm::vec3(1, 1, 0);
const glm::vec3 red = glm::vec3(1, 0, 0);
const glm::vec3 orange = glm::vec3(1, 0.5, 0);
const glm::vec3 green = glm::vec3(0, 1, 0);
const glm::vec3 blue = glm::vec3(0, 0, 1);
const glm::vec3 teal = glm::vec3(0, 1, 1);
const glm::vec3 grey = glm::vec3(.5, .5, .5);

class Preset {

public:

    // Tools

    void cubicGrid(glm::vec3 cornerPosition = glm::vec3(-100, -100, -200), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, -100.0f),
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
