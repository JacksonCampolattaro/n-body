//
// Created by jackcamp on 10/25/18.
//

#include <GL/glew.h>
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

    std::cout << "Creating Bodies \n \n";

    orbitalBody *testBody1 = new orbitalBody(glm::vec3(0, 0, -100), glm::vec3(0, 0, 0), 10000, 1);
    orbitalBody *testBody2 = new orbitalBody(glm::vec3(50, 0, -100), glm::vec3(0, 1, 0), 1000, 1);

    testBody1->soundOff();
    testBody2->soundOff();

    std::cout << "Creating Simulation and Adding Bodies \n \n";

    auto testSimulationState = new simulationState(.03, 1);

    testSimulationState->addBody(testBody1);
    testSimulationState->addBody(testBody2);

    std::cout << "Incrementing Simulation \n \n";

    testSimulationState->increment();

    testBody1->soundOff();
    testBody2->soundOff();

    // Testing GLFW
    auto testViewport = new viewport(testSimulationState);

}