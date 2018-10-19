
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include statements
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Make sure to install the necessary graphics packages before using:
// glew glew-devel SDL2-devel SDL2_image-devel glm-devel freetype-devel glut freeglut-devel glm-devel
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>

// Other libraries to include
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string>
#include <omp.h>

// Libraries from this project
#include "simulation.h"
#include "body.h"




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global Variables & Configurables
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Turns off status readout
bool silent = true;

// Activates color coding by velocity.
bool colorByVelocity = false;

// Sets the size of the window
ivec2 windowSize(9000, 9000);

// Higher --> more exaggerated gravity
float accelerationOfGravity = 0.3;

// Lower --> slower & more accurate physics
float physicsTimeInterval = .01;

// Higher --> smaller objects
float density = 40.0f;

// Initializes the simulation to be used by the program.
simulation mySim(accelerationOfGravity, physicsTimeInterval);

// Colors to use
vec3 white = vec3(1, 1, 1);
vec3 yellow = vec3(1, 1, 0);
vec3 red = vec3(1, 0, 0);
vec3 green = vec3(0, 1, 0);
vec3 blue = vec3(0, 0, 1);
vec3 teal = vec3(0, 1, 1);
vec3 grey = vec3(.5, .5, .5);

// Use this function to add bodies to the simulation
void addBodies() {

    cout << omp_get_max_threads();

    // Superheavy body
    /*
    mySim.addBody(0, 0, -800, 0, 0, 0, 15000000, density, yellow, true);
    //*/

    // Binary System
    /*
    mySim.addBody(0.0, 60.0, -300, 30, 0, 0, 500000, density, yellow);
    mySim.addBody(0.0, -60, -300, -30, 0, 0, 500000, density, red);
    //*/

    // Three Body System
    ///*
    mySim.addBody(0.0, 60.0, -100, 30, 0, 0, 500000, density, red);
    mySim.addBody(0.0, -60, -300, -30, 0, 0, 500000, density, white);
    mySim.addBody(-50, 0, -400, 0, 0, 0, 500000, density, yellow);
    //*/

    // Horizontal Binary System
    /*
    mySim.addBody(20.0, 0, -400, 50, 0, 0, 500000, density, yellow);
    mySim.addBody(-20.0, 0, -200, -50, 0, 0, 500000, density, red);
    //*/

    // Cubic Grid
    /*
    vec3 cornerPosition(-50, 150, -1000);
    vec3 velocity(-800.0f, 0.0f, -0.0f);
    vec3 size(10, 10, 10);
    float spacing = 10.0f;
    float mass = 50.0f;

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

                mySim.addBody(position, velocity, mass, density, blueToRedToWhite, false);
            }
        }
    }
    //*/


}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graphics main loop and rendering functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Changes size of the output if the window is resized.
void handleResize(int w, int h) {

    // Changing the window size
    glViewport(0, 0, w, h);

    // Resetting the camera perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float) w / (float) h, 1.0, 10000.0);
}

// Draws a circle of a given position and radius
void drawCircle(vec3 position, float radius) {

    // Number of segments defines how 'smooth' the circle needs to be. these are far away, so it doesn't need to be too high.
    int numSegments = 36;


    glBegin(GL_TRIANGLE_FAN);

    for (int i = 0; i < numSegments; ++i) {

        float angle = (2.0f * 3.1415926f * float(i)) / float(numSegments);

        float x = radius * cosf(angle);
        float y = radius * sinf(angle);

        glVertex3f(x + position.x, y + position.y, position.z);
    }

    glEnd();
}

// sets the color given a 3d color vector
void setColor(vec3 color) {
    glColor3f(color.r, color.g, color.b);
}

// Draws the body passed to it with all its attributes
void drawBody(body thisBody) {

    if (colorByVelocity) {
        vec3 velocityColor(thisBody.getVelocity() + 0.1f);
        setColor(velocityColor);
    } else {
        setColor(thisBody.getColor());
    }
    drawCircle(thisBody.getPosition(), thisBody.getRadius());

}

// Outputs text at the given location and with the given color
void drawText(ivec2 position, string text, vec3 color) {

    // Prepares for 2d drawing
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Sets the color
    glColor3f(color.r, color.g, color.b);

    // Sets the position
    glRasterPos2i(position.x, position.y);

    // Sets the font
    void *font = GLUT_BITMAP_9_BY_15;

    // Draws the text
    for (string::iterator i = text.begin(); i != text.end(); ++i) {
        char c = *i;
        glutBitmapCharacter(font, c);
    }


    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);

    //glReadPixels(0, 0, GLUT_WINDOW_WIDTH, GLUT_WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, GL_BGRA);

};

// This code will be repeated by the specified number of milliseconds
void update(int value) {

    // Applies Physics
    mySim.incrementPhysics();

    // Outputs all information for diagnostics
    if (!silent) {
        mySim.soundOff();
    }

    // Tell GLUT that the scene has changed
    glutPostRedisplay();

    // Tell GLUT to call update again after the specified number of milliseconds
    glutTimerFunc(1, update, 0);
}

// Primary graphics loop, this is where most other functions are called from.
void draw() {

    // Clear information and sets up the 3d model world
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glLoadIdentity();


    // Retrieves the list of bodies
    vector<body *> bodies = mySim.getBodies();

    // Draws each of the bodies.
    for (body *thisBody : bodies) {
        drawBody(*thisBody);
    }

    // Displays information about the current simulation for optimization purposes
    string physicsIteration = "Physics Iteration: " + to_string(mySim.getPhysicsIteration());
    drawText(ivec2(10, 50), physicsIteration, vec3(0.5, 0.5, 0.5));
    string elapsedTime = "     Elapsed Time: " + to_string(int(glutGet(GLUT_ELAPSED_TIME) / 1000.0f)) + " seconds";
    drawText(ivec2(10, 30), elapsedTime, vec3(0.5, 0.5, 0.5));
    string calcRate =
            "Iterations/Second: " + to_string(1000.0f * mySim.getPhysicsIteration() / glutGet(GLUT_ELAPSED_TIME));
    drawText(ivec2(10, 10), calcRate, vec3(1, 1, 1));

    // Output to the screen
    glutSwapBuffers();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Loop
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {


    glutInit(&argc, argv);

    // Sets up the graphics window
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(windowSize.x, windowSize.y);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("n body");

    glutDisplayFunc(draw);
    glutReshapeFunc(handleResize);

    glutTimerFunc(5, update, 0);

    addBodies();

    glutMainLoop();
}