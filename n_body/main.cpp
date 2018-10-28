//
// Created by jackcamp on 10/25/18.
//

#include "orbitalBody.h"
#include "simulationState.h"
#include "viewport.h"


// Colors to use
vec3 white = vec3(1, 1, 1);
vec3 yellow = vec3(1, 1, 0);
vec3 red = vec3(1, 0, 0);
vec3 green = vec3(0, 1, 0);
vec3 blue = vec3(0, 0, 1);
vec3 teal = vec3(0, 1, 1);
vec3 grey = vec3(.5, .5, .5);

float density = 30;

simulationState *theSimulation;


void addBodies() {

    // Three body simulation
    ///*
    theSimulation->addBody( new orbitalBody(vec3(8.0, 60.0, -100), vec3(30, 0, 0), 500000, density, red));
    theSimulation->addBody( new orbitalBody(vec3(0.0, -60, -300), vec3(-30, 0, 0), 500000, density, white));
    theSimulation->addBody( new orbitalBody(vec3(-8, 0, -400), vec3(0, 0, 0), 500000, density, yellow));
    //*/

    // Massive fixed mass
    /*
    theSimulation->addBody(new orbitalBody(vec3(0, 0, -400), vec3(0, 0, 0), 15000000, density, yellow, true));
    //*/

    // Cubic Grid
    /*
    vec3 cornerPosition(-50, 150, -600);
    vec3 velocity(-800.0f, 0.0f, -0.0f);
    vec3 size(10, 10, 5);
    float spacing = 10.0f;
    float mass = 1000.0f;

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            for (int z = 0; z < size.z; ++z) {

                vec3 coordinate(x, y, z);
                vec3 position = cornerPosition + (coordinate * spacing);
                //velocity.x = 200.0f * (coordinate.z ) - 1000.0f;

                vec3 multiColor = coordinate / size + 0.2f;
                vec3 yellowToRed(1, coordinate.y / size.y, 0);
                vec3 greenToWhite(coordinate.y / size.y, 1, coordinate.y / size.y);
                vec3 blueToRed(coordinate.y / size.y, 0, 1 - coordinate.y / size.y);
                vec3 blueToRedToWhite(coordinate.y / size.y + coordinate.x / size.x, coordinate.x / size.x,
                                      1 - coordinate.y / size.y + coordinate.x / size.x);
                vec3 rainbow(coordinate.y / size.y, coordinate.x / size.x, 1 - coordinate.y / size.y);

                theSimulation->addBody(new orbitalBody(position, velocity, mass, density, blueToRedToWhite, false));
            }
        }
    }
    //*/
}

int main(int argc, char **argv) {

    // Creating simulation
    theSimulation = new simulationState(.2, .001, 1);

    // Adding bodies
    addBodies();

    // Testing GLFW
    auto theViewport = new viewport(theSimulation, 3000, 1500);

}