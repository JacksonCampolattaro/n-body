//
// Created by jackcamp on 4/11/19.
//

#include <GL/gl.h>
#include <GL/glu.h>
#include "GLToolkit.h"

void GLToolkit::setColor(glm::vec3 color) {

    // Sets the color used in the next OpenGL action
    glColor3f(color.r, color.g, color.b);
}

void GLToolkit::drawCircle(glm::vec3 position, float radius) {

    // Number of segments affects how 'smooth' the circle will be.
    int numSegments = 64;

    glBegin(GL_TRIANGLE_FAN);

    for (int i = 0; i < numSegments; ++i) {

        float angle = (2.0f * 3.1415926f * float(i)) / float(numSegments);

        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex3f(x + position.x, y + position.y, position.z);
    }

    glEnd();
}

void GLToolkit::drawSphere(glm::vec3 position, float radius) {

    GLUquadric *quad = gluNewQuadric();
    glTranslatef(position.x, position.y, position.z);
    gluSphere(quad, radius, 50, 10);
    glTranslatef(-position.x, -position.y, -position.z);

}
