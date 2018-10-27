//
// Created by jackcamp on 10/25/18.
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string>
#include <omp.h>

#include "orbitalBody.h"
#include "relationship.h"
#include "simulationState.h"
#include "viewport.h"

int main(int argc, char **argv) {

    // Creating Bodies
    orbitalBody *testBody1 = new orbitalBody(glm::vec3(0, 0, -1000), glm::vec3(0, 0, 0), 10000000, 1, vec3(1, 0, 1),
                                             true);
    orbitalBody *testBody2 = new orbitalBody(glm::vec3(50, 0, -100), glm::vec3(0, 0, 0), 10000, 1);

    // Creating simulation
    auto testSimulationState = new simulationState(.03, 1, 2);

    // Adding bodies
    testSimulationState->addBody(testBody1);
    testSimulationState->addBody(testBody2);

    // Testing GLFW
    auto testViewport = new viewport(testSimulationState, 3000, 1500);

}