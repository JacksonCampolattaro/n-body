//
// Created by jackcamp on 10/25/18.
//

#include "model/Body.h"
#include "view/View.h"
#include "model/calculation/Solver.h"
#include "model/calculation/Naive/NaiveSolver.h"
#include "model/calculation/BarnesHut/BarnesHutSolver.h"
#include "model/Model.h"

#include <glm/glm.hpp>

/*#include <gtest/gtest.h>
#include <omp.h>*/


// Useful constants

const glm::vec3 white = glm::vec3(1, 1, 1);
const glm::vec3 yellow = glm::vec3(1, 1, 0);
const glm::vec3 red = glm::vec3(1, 0, 0);
const glm::vec3 green = glm::vec3(0, 1, 0);
const glm::vec3 blue = glm::vec3(0, 0, 1);
const glm::vec3 teal = glm::vec3(0, 1, 1);
const glm::vec3 grey = glm::vec3(.5, .5, .5);

float density = 40;


// Objects used for the simulation
View *view;
Model *model;
PhysicsContext *physics;
std::vector<Body *> bodies;

void cubicGrid(glm::vec3 cornerPosition = glm::vec3(-100, -100, -200), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, -100.0f),
               glm::vec3 size = glm::vec3(20, 20, 20), float spacing = 10.0f, float mass = 10000.0f) {

    density = 40;

    // Creates a cubic grid

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            for (int z = 0; z < size.z; ++z) {

                glm::vec3 coordinate(x, y, z);
                glm::vec3 position = cornerPosition + (coordinate * spacing);

                if (x < 0) {
                    velocity *= -1;
                }

                // Color options
                glm::vec3 multiColor = coordinate / size + 0.2f;
                glm::vec3 yellowToRed(1, coordinate.y / size.y, 0);
                glm::vec3 yellowToRed_Z(1, coordinate.z / size.z, 0);
                glm::vec3 greenToWhite(coordinate.y / size.y, 1, coordinate.y / size.y);
                glm::vec3 blueToRed(coordinate.y / size.y, 0, 1 - coordinate.y / size.y);
                glm::vec3 blueToRed_Z(coordinate.z / size.z, 0, 1 - coordinate.z / size.z);
                glm::vec3 blueToRedToWhite(coordinate.y / size.y + coordinate.x / size.x, coordinate.x / size.x,
                                      1 - coordinate.y / size.y + coordinate.x / size.x);
                glm::vec3 whiteToBlueToRed(coordinate.y / size.y + coordinate.z / size.z, coordinate.z / size.z,
                                      1 - coordinate.y / size.y + coordinate.z / size.z);
                glm::vec3 rainbow(coordinate.y / size.y, coordinate.x / size.x, 1 - coordinate.y / size.y);


                auto newBody = new Body(position);
                newBody->setVelocity(velocity)->setMass(mass)->setDensity(40)->setColor(yellowToRed_Z);
                bodies.push_back(newBody);
            }
        }
    }
}


void bigDemo() {

    physics->setT(0.005)->setG(0.01)->setPower(2);

    // Massive fixed mass
    auto superHeavy = new Body(glm::vec3(0, 0, -500));
    superHeavy->setMass(180000000)->setColor(white)->setDensity(10000)->makeFixed();
    bodies.push_back(superHeavy);

    // Cubic Grid
    glm::vec3 cornerPosition(-50, -100, -450);
    glm::vec3 velocity(60.0f, 20.0f, 0.0f);
    glm::vec3 size(6, 6, 30);
    float spacing = 10.0f;
    float mass = 5000.0f;

    cubicGrid(cornerPosition, velocity, size, spacing, mass);
}


void threeBodyDemo() {
    // Three Body Model

    physics->setPower(1);
    physics->setG(.2);
    auto redBody = new Body(glm::vec3(20.0, 50.0, -300));
    redBody->setVelocity(glm::vec3(30, -50, 0))->setMass(500000)->setDensity(density)->setColor(red);
    bodies.push_back(redBody);
    auto whiteBody = new Body(glm::vec3(0.0, -50, -400));
    whiteBody->setVelocity(glm::vec3(-30, 0, 0))->setMass(500000)->setDensity(density)->setColor(white);
    bodies.push_back(whiteBody);
    auto yellowBody = new Body(glm::vec3(-20, 0, -700));
    yellowBody->setVelocity(glm::vec3(0, 50, 0))->setMass(500000)->setDensity(density)->setColor(yellow);
    bodies.push_back(yellowBody);
}


void addBodies() {

    // Orbit Model

    /*auto sun = new Body(glm::vec3(0, 0, -4000), glm::vec3(0, 0, 0), 50000000, density, yellow, true);
    addBody(sun);

    auto earth = new Body(glm::vec3(2000, 0, -4000), glm::vec3(0, 1, 0), 4000000, density, blue);
    addBody(earth);
    theSimulation->orbit(sun, earth);

    auto moon = new Body(glm::vec3(2000, -130, -4000), glm::vec3(0, 0, 1), 500, 0.05, white);
    theSimulation->orbit(earth, moon);
    moon->kick(earth->getVelocity());
    addBody(moon);*/

    /*auto earth = new Body(glm::vec3(0, 0, -100));
    earth->setMass(50000)->setColor(blue);
    addBody(earth);

    auto moon = new Body(glm::vec3(0, -50, -200));
    moon->setMass(500)->setVelocity(glm::vec3(10, 0, 0))->setColor(white);
    addBody(moon);*/

    bigDemo();
    //density = 30; cubicGrid(glm::vec3(-5, -5, -100), glm::vec3(0, 0, -25), glm::vec3(2, 2, 2), 10, 5000);
    //cubicGrid(glm::vec3(-95, -95, -100), glm::vec3(0, 0, -25), glm::vec3(20, 20, 20), 10, 5000);
    //threeBodyDemo();

    //cubicGrid(glm::vec3(-50, -50, -500), glm::vec3(100, 0, 0), glm::vec3(10, 10, 10));
    //cubicGrid(glm::vec3(-50, 50, -500), glm::vec3(-100, 0, 0), glm::vec3(10, 10, 10));
}

int main(int argc, char **argv) {

    // Creating the physics world
    physics = new PhysicsContext();
    physics->setT(0.001)->setG(0.02)->setPower(1);

    // Adding bodies to the simulation
    addBodies();

    // Creating the solving tool
    auto *solver = new BarnesHutSolver();
    solver->setTheta(0)->enableThreading();

    // Creating the view
    view = new View();
    view->setTitle("Jackson Campolattaro's n-body Simulator");

    // Adding bodies to the viewport
    for (Body *b : bodies) {
        view->registerDrawable(b);
    }

    // Creating the model
    model = new Model(physics, solver);
    model->preCalculate(bodies);

    // Incrementing the simulation
    for (int i = 0; i < 100000000; ++i) {
        view->draw();
        model->increment(bodies);
    }

}