//
// Created by jackcamp on 10/25/18.
//

#ifndef N_BODY_HEADLESS_RELATIONSHIP_H
#define N_BODY_HEADLESS_RELATIONSHIP_H

#include <glm/glm.hpp>
#include "body.h"

static const float maximumAllowableAcceleration = 1000;
static const float timeResolutionLimit = 0.0001;

class relationship {

public:

    /**
     * Creates a new relationship between two bodies
     *
     * @param firstBody one body
     * @param secondBody another body
     */
    relationship(body *firstBody, body *secondBody);

    /**
     * Applies the gravity between the two bodies according to the standard equation.
     *
     * @param interval the time interval over which gravity should be applied
     * @param gravitationalConstant the constant of gravity 'G'
     * @param power the power of the distance factor (e.g Gmm/d^2 vs Gmm/d^1)
     */
    void applyGravity(float interval, float gravitationalConstant = 0.03, int power = 2);

    /**
     * Converts the relationship to a string
     * @return a string describing both bodies of the relationship
     */
    std::string toString();

private:

    /**
     * Calculates the distance between the two bodies
     * @return the distance as a float
     */
    float distance();

    /**
     * Creates a unit vector which represents the relationship between the bodies
     *
     * @return unit vector of the line between the bodies
     */
    vec3 unitVector();

    body *firstBody;
    body *secondBody;
};


#endif //N_BODY_HEADLESS_RELATIONSHIP_H
