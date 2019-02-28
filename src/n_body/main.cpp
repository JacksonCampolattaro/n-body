//
// Created by jackcamp on 10/25/18.
//

#include "tracker.h"
#include "model/body.h"
#include "model/simulation.h"
#include "interface/viewport.h"

#include <glm/glm.hpp>
#include <gtest/gtest.h>
#include <omp.h>


// Colors to use
static glm::vec3 white = glm::vec3(1, 1, 1);
static glm::vec3 yellow = glm::vec3(1, 1, 0);
static glm::vec3 red = glm::vec3(1, 0, 0);
static glm::vec3 green = glm::vec3(0, 1, 0);
static glm::vec3 blue = glm::vec3(0, 0, 1);
static glm::vec3 teal = glm::vec3(0, 1, 1);
static glm::vec3 grey = glm::vec3(.5, .5, .5);

float density = 40;

simulation *theSimulation;
viewport *theViewport;


void addBody(body *newBody) {
    
    theSimulation->addBody(newBody);
    theViewport->registerDrawable(newBody);
}

void cubicGrid(glm::vec3 cornerPosition = glm::vec3(-100, -100, -200), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, -100.0f),
               glm::vec3 size = glm::vec3(20, 20, 20), float spacing = 10.0f, float mass = 10000.0f) {

    theSimulation->setPower(2);

    // Creates a cubic grid

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            for (int z = 0; z < size.z; ++z) {

                glm::vec3 coordinate(x, y, z);
                glm::vec3 position = cornerPosition + (coordinate * spacing);

                if (x < 0) {
                    velocity *= -1;
                }

                glm::vec3 multiColor = coordinate / size + 0.2f;
                glm::vec3 yellowToRed(1, coordinate.z / size.z, 0);
                glm::vec3 greenToWhite(coordinate.y / size.y, 1, coordinate.y / size.y);
                glm::vec3 blueToRed(coordinate.y / size.y, 0, 1 - coordinate.y / size.y);
                glm::vec3 blueToRedToWhite(coordinate.y / size.y + coordinate.x / size.x, coordinate.x / size.x,
                                      1 - coordinate.y / size.y + coordinate.x / size.x);
                glm::vec3 rainbow(coordinate.y / size.y, coordinate.x / size.x, 1 - coordinate.y / size.y);


                auto newBody = new body(position);
                newBody->setVelocity(velocity)->setMass(mass)->setDensity(40)->setColor(yellowToRed);
                addBody(newBody);
            }
        }
    }
}


void bigDemo() {

    // Massive fixed mass
    auto superHeavy = new body(glm::vec3(0, 0, -500));
    superHeavy->setMass(180000000)->setColor(white)->setDensity(10000)->makeFixed();
    addBody(superHeavy);

    // Cubic Grid
    glm::vec3 cornerPosition(-50, -100, -450);
    glm::vec3 velocity(60.0f, 20.0f, 0.0f);
    glm::vec3 size(6, 6, 100);
    float spacing = 10.0f;
    float mass = 5000.0f;

    cubicGrid(cornerPosition, velocity, size, spacing, mass);
}


/*void threeBodyDemo() {
    // Three body model

    theSimulation->setPower(2);
    theSimulation->setGravitationalConstant(2);
    theSimulation->setTimeInterval(.01);
    addBody(new body(glm::vec3(20.0, 50.0, -300), glm::vec3(30, -50, 0), 500000, density, red));
    addBody(new body(glm::vec3(0.0, -50, -400), glm::vec3(-30, 0, 0), 500000, density, white));
    addBody(new body(glm::vec3(-20, 0, -700), glm::vec3(0, 50, 0), 500000, density, yellow));
}*/


void addBodies() {

    // Orbit model

    density = 10;

    /*auto sun = new body(glm::vec3(0, 0, -4000), glm::vec3(0, 0, 0), 50000000, density, yellow, true);
    addBody(sun);

    auto earth = new body(glm::vec3(2000, 0, -4000), glm::vec3(0, 1, 0), 4000000, density, blue);
    addBody(earth);
    theSimulation->orbit(sun, earth);

    auto moon = new body(glm::vec3(2000, -130, -4000), glm::vec3(0, 0, 1), 500, 0.05, white);
    theSimulation->orbit(earth, moon);
    moon->kick(earth->getVelocity());
    addBody(moon);*/

    /*auto earth = new body(glm::vec3(0, 0, -100));
    earth->setMass(50000)->setColor(blue);
    addBody(earth);

    auto moon = new body(glm::vec3(0, -50, -200));
    moon->setMass(500)->setVelocity(glm::vec3(10, 0, 0))->setColor(white);
    addBody(moon);*/

    bigDemo(); theSimulation->setT(0.005);
    //density = 30; cubicGrid(glm::vec3(-95, -95, -200), glm::vec3(0, 0, -25), glm::vec3(20, 20, 20), 10, 5000);
    //threeBodyDemo();

    //cubicGrid(glm::vec3(-50, -50, -500), glm::vec3(100, 0, 0), glm::vec3(10, 10, 10));
    //cubicGrid(glm::vec3(-50, 50, -500), glm::vec3(-100, 0, 0), glm::vec3(10, 10, 10));
}

int main(int argc, char **argv) {

    theSimulation = new simulation();
    theViewport = new viewport();

    theSimulation->setG(0.01)->setT(0.01)->enableBarnesHut()->setTheta(0.9)->enableLeapfrog()->enableThreading();
    theViewport->setTitle("n body simulator")->attachSimulation(theSimulation);

    addBodies();

    theViewport->start();

}