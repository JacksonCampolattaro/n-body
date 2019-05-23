//
// Created by jackcamp on 10/25/18.
//

#include "model/Body.h"
#include "view/View.h"
#include "model/calculation/Solver.h"
#include "model/calculation/Naive/NaiveSolver.h"
#include "model/calculation/BarnesHut/BarnesHutSolver.h"
#include "model/Model.h"
#include "view/Observer.h"
#include "view/ViewPort.h"

#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>
#include <algorithm>

/*#include <gtest/gtest.h>
#include <omp.h>*/

using std::cout;
using std::endl;


// Useful constants

const glm::vec3 white = glm::vec3(1, 1, 1);
const glm::vec3 yellow = glm::vec3(1, 1, 0);
const glm::vec3 red = glm::vec3(1, 0, 0);
const glm::vec3 orange = glm::vec3(1, 0.5, 0);
const glm::vec3 green = glm::vec3(0, 1, 0);
const glm::vec3 blue = glm::vec3(0, 0, 1);
const glm::vec3 teal = glm::vec3(0, 1, 1);
const glm::vec3 grey = glm::vec3(.5, .5, .5);

float density = 40;


// Objects used for the simulation
View *view;
Observer *output;
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
                glm::vec3 blueToGreen(0, 1 - coordinate.z / size.z, coordinate.z / size.z);
                glm::vec3 rainbow(coordinate.y / size.y, coordinate.x / size.x, 1 - coordinate.y / size.y);


                auto newBody = new Body(position);
                newBody->setVelocity(velocity)->setMass(mass)->setDensity(40)->setColor(whiteToBlueToRed);
                bodies.push_back(newBody);
            }
        }
    }
}


void blender() {

    physics->setT(0.005)->setG(0.01)->setPower(2);

    // Massive fixed mass
    auto superHeavy = new Body(glm::vec3(-60, 0, -500));
    superHeavy->setMass(180000000)->setColor(orange)->setDensity(10000)->setVelocity(glm::vec3(0, -90, 0));
    bodies.push_back(superHeavy);

    auto superHeavy2 = new Body(glm::vec3(60, 0, -500));
    superHeavy2->setMass(180000000)->setColor(orange)->setDensity(10000)->setVelocity(glm::vec3(0, 90, 0));
    bodies.push_back(superHeavy2);

    // Cubic Grid
    glm::vec3 cornerPosition(-50, -100, -450);
    glm::vec3 velocity(60.0f, 20.0f, 0.0f);
    glm::vec3 size(6, 6, 80);
    float spacing = 10.0f;
    float mass = 5000.0f;

    cubicGrid(cornerPosition, velocity, size, spacing, mass);
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
    glm::vec3 size(6, 6, 80);
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

    //bigDemo();
    blender();
    //threeBodyDemo();

    //density = 30; cubicGrid(glm::vec3(-5, -5, -100), glm::vec3(0, 0, -25), glm::vec3(2, 2, 2), 10, 5000);
    //cubicGrid(glm::vec3(-95, -95, -100), glm::vec3(0, 0, -15), glm::vec3(20, 20, 10), 10, 5000);

    //cubicGrid(glm::vec3(-50, -50, -500), glm::vec3(100, 0, 0), glm::vec3(10, 10, 10));
    //cubicGrid(glm::vec3(-50, 50, -500), glm::vec3(-100, 0, 0), glm::vec3(10, 10, 10));
}

int main(int argc, char **argv) {

    // Creating the default physics world
    physics = new PhysicsContext();
    physics->setT(0.005)->setG(0.02)->setPower(2);
    physics->enableForceSoftening()->setMinimumT(physics->getT() / 100);

    // Adding bodies to the simulation
    addBodies();

    // Creating the solving tool
    auto *solver = new BarnesHutSolver();
    solver->enableThreading();
    solver->setTheta(0.8);

    // Creating the view
    //view = new View();
    //view->setTitle("Jackson Campolattaro's n-body Simulator");
    output = new ViewPort();
    output->setTitle("n_body")->setDimensions(glm::ivec2(3440, 1440));

    /*// Adding bodies to the viewport
    for (Body *b : bodies) {
        //view->registerDrawable(b);
    }*/

    // Creating the model
    model = new Model(physics, solver);
    model->preCalculate(bodies); // Enables leapfrog integration


    // Tracking
    clock_t startTime, endTime;
    double drawTime, calcTime;
    cout << std::fixed << std::setprecision(3); // << std::setfill('0') << std::setw(2);

    // Recording
    const char* cmd = "ffmpeg -r 30 -f rawvideo -pix_fmt rgba -s 3440x1440 -i - "
                      "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 19 -vf vflip /home/jackcamp/CLionProjects/n_body/src/n_body/staging/output.mp4";
    FILE *ffmpeg = popen(cmd, "w");

    // Incrementing the simulation
    for (int i = 0; i < 4000; ++i) {

        startTime = clock();
        output->draw(*(std::vector<Drawable*> *)&bodies); // A copy of the list of bodies is passed to the renderer
        endTime = clock();
        drawTime = double(endTime - startTime) / CLOCKS_PER_SEC;

        startTime = clock();
        model->increment(bodies);
        endTime = clock();
        calcTime = double(endTime - startTime) / CLOCKS_PER_SEC;

        cout << "Drawing:     " << drawTime << "s (" << 1 / drawTime << "hz)" << endl;
        cout << "Calculation: " << calcTime << "s (" << 1 / calcTime << "hz)" << endl;
        cout << "Total:       " << (drawTime + calcTime) << "s (" << 1 / (drawTime + calcTime) << "hz)" << endl;
        for (int k = 0; k < drawTime * 100; ++k) {
            cout << "o";
        }
        for (int j = 0; j < calcTime * 100; ++j) {
            cout << "+";
        }
        cout << endl;


        /*// FIXME This isn't actually being done in parallel!
        startTime = clock();
        std::vector<Drawable *> drawables = *(std::vector<Drawable *> *) &bodies;
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                // A copy of the list of bodies is passed to the renderer
                output->draw(drawables);
            }
            #pragma omp section
            {
                model->increment(bodies);
            }
        }

        endTime = clock();
        calcTime = double(endTime - startTime) / CLOCKS_PER_SEC;
        cout << calcTime << endl;*/


        // TODO Rendering
        int *buffer = new int[3440 * 1440];
        glReadPixels(0, 0, 3440, 1440, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        fwrite(buffer, sizeof(int)*3440*1440, 1, ffmpeg);
        delete buffer;

    }

    pclose(ffmpeg);

}