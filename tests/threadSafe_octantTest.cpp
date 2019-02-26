//
// Created by jackcamp on 2/24/19.
//

#include <gtest/gtest.h>
#include <exception>
#include "../src/n_body/model/octant.h"
#include "../src/n_body/tracker.h"


class threadSafe_octantTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        theOctant = new threadSafe_octant(glm::vec3(0, 0, 0), 16);
        tracker::instance();
    }

    virtual void TearDown() {
        delete theOctant;
    }

    threadSafe_octant *theOctant;
};

TEST_F(threadSafe_octantTest, threadSafe_octantTest_addBody_Test) {

    // Adding one body to the octant
    theOctant->addBody(vec3(1, 1, 1), 100);
    EXPECT_EQ(1, theOctant->getNumBodies());
    EXPECT_EQ(vec3(1, 1, 1), theOctant->getCenterOfMass());
    EXPECT_EQ(vec3(1, 1, 1), theOctant->getAveragePosition());
    cout << theOctant->toString();

    // Adding a second body to the octant and finding a center of mass between the two
    theOctant->addBody(vec3(1, 1, -1), 100);
    EXPECT_EQ(2, theOctant->getNumBodies());
    EXPECT_EQ(vec3(1, 1, 0), theOctant->getCenterOfMass());
    EXPECT_EQ(vec3(1, 1, 0), theOctant->getAveragePosition());
    cout << theOctant->toString();

    // Adding a third body to the octant and finding an non-integer center of mass
    theOctant->addBody(vec3(0, 0, 0), 100);
    EXPECT_EQ(3, theOctant->getNumBodies());
    EXPECT_FLOAT_EQ(2.0f/3.0f, theOctant->getCenterOfMass().x);
    EXPECT_FLOAT_EQ(2.0f/3.0f, theOctant->getCenterOfMass().y);
    EXPECT_FLOAT_EQ(0, theOctant->getCenterOfMass().z);


    /*// Adding one body to the octant
    theOctant->addBody(new body(vec3(1, 1, 1), vec3(0, 0, 0), 100));
    EXPECT_EQ(1, theOctant->numBodies());
    theOctant->calculateCenterMass();
    EXPECT_EQ(vec3(1, 1, 1), theOctant->getCenterMass()->getPosition());
    EXPECT_EQ(100, theOctant->getCenterMass()->getMass());

    // Adding a second body to the octant and finding a center of mass between the two
    theOctant->addBody(new body(vec3(1, 1, -1), vec3(0, 0, 0), 100));
    EXPECT_EQ(2, theOctant->numBodies());
    theOctant->calculateCenterMass();
    EXPECT_EQ(vec3(1, 1, 0), theOctant->getCenterMass()->getPosition());

    // Adding a third body to the octant and finding an non-integer center of mass
    theOctant->addBody(new body(vec3(0, 0, 0), vec3(0, 0, 0), 100));
    EXPECT_EQ(3, theOctant->numBodies());
    theOctant->calculateCenterMass();
    EXPECT_FLOAT_EQ(2.0f/3.0f, theOctant->getCenterMass()->getPosition().x);
    EXPECT_FLOAT_EQ(2.0f/3.0f, theOctant->getCenterMass()->getPosition().y);
    EXPECT_FLOAT_EQ(0, theOctant->getCenterMass()->getPosition().z);

    // Finding a center of mass when not all masses are the same
    theOctant->addBody(new body(vec3(0, 0, -1), vec3(0, 0, 0), 500));
    theOctant->calculateCenterMass();
    EXPECT_FLOAT_EQ(1.0f/4.0f, theOctant->getCenterMass()->getPosition().x);
    EXPECT_FLOAT_EQ(1.0f/4.0f, theOctant->getCenterMass()->getPosition().y);
    EXPECT_FLOAT_EQ(-5.0f/8.0f, theOctant->getCenterMass()->getPosition().z);

    cout << "\n**** toString Output ****\n" << theOctant->toString() << "\n\n";*/
}

