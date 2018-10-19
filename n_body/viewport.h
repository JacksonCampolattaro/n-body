//
// Created by jackcamp on 10/19/18.
//

#ifndef N_BODY_HEADLESS_VIEWPORT_H
#define N_BODY_HEADLESS_VIEWPORT_H


#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>

/**
 * Responsible for using glut methods to draw the window
 */
class viewport {

public:

    /**
     * viewport Constructor
     */
    viewport(int argc, char **argv) {

        // Starting glut
        glutInit(&argc, argv);

        // Sets up the graphics window
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

        // Configuring the window
        glutInitWindowSize(windowSize.x, windowSize.y);
        glutInitWindowPosition(100, 100);
        glutCreateWindow("n body");

        // Tells glut which method to use to update
        glutTimerFunc(5, update, 0);

        // Tells glut which method to use to draw
        glutDisplayFunc(this->draw);

        // Tells glut which method to use when the window is resized
        glutReshapeFunc(this->handleResize);

        // Starts glut
        glutMainLoop();

    }

    /**
     * Handles resizing of the window
     *
     * @param w the new width of the window
     * @param h the new height of the window
     */
    void handleResize(int w, int h) {

        // Changing the window size
        glViewport(0, 0, w, h);

        // Resetting the camera perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Setting camera location
        gluPerspective(45.0, (float) w / (float) h, 1.0, 10000.0);
    }

    /**
     * Draws the simulation to the window
     */
    void draw() {

    }
};


#endif //N_BODY_HEADLESS_VIEWPORT_H
