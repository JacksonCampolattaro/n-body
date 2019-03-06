//
// Created by jackcamp on 2/24/19.
//

#include <gtest/gtest.h>
#include <exception>
#include "../src/n_body/model/Octant.h"
#include "../src/n_body/Tracker.h"


class octantTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        theOctant = new Octant(glm::vec3(0, 0, 0), 16);
        Tracker::instance();
    }

    virtual void TearDown() {
        delete theOctant;
    }

    Octant *theOctant;
};

TEST_F(octantTest, octantTest_addBody_Test) {

    // Adding one Body to the Octant
    theOctant->addBody(glm::vec3(1, 1, 1), 100);
    EXPECT_EQ(1, theOctant->getNumBodies());
    EXPECT_EQ(glm::vec3(1, 1, 1), theOctant->getCenterOfMass());
    EXPECT_EQ(glm::vec3(1, 1, 1), theOctant->getAveragePosition());
    std::cout << theOctant->toString();

    // Adding a second Body to the Octant and finding a center of mass between the two
    theOctant->addBody(glm::vec3(1, 1, -1), 100);
    EXPECT_EQ(2, theOctant->getNumBodies());
    EXPECT_EQ(glm::vec3(1, 1, 0), theOctant->getCenterOfMass());
    EXPECT_EQ(glm::vec3(1, 1, 0), theOctant->getAveragePosition());
    std::cout << theOctant->toString();

    // Adding a third Body to the Octant and finding an non-integer center of mass
    theOctant->addBody(glm::vec3(0, 0, 0), 100);
    EXPECT_EQ(3, theOctant->getNumBodies());
    EXPECT_FLOAT_EQ(2.0f/3.0f, theOctant->getCenterOfMass().x);
    EXPECT_FLOAT_EQ(2.0f/3.0f, theOctant->getCenterOfMass().y);
    EXPECT_FLOAT_EQ(0, theOctant->getCenterOfMass().z);


    /*// Adding one Body to the Octant
    theOctant->addBody(new Body(glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 100));
    EXPECT_EQ(1, theOctant->numBodies());
    theOctant->calculateCenterMass();
    EXPECT_EQ(glm::vec3(1, 1, 1), theOctant->getCenterMass()->getPosition());
    EXPECT_EQ(100, theOctant->getCenterMass()->getMass());

    // Adding a second Body to the Octant and finding a center of mass between the two
    theOctant->addBody(new Body(glm::vec3(1, 1, -1), glm::vec3(0, 0, 0), 100));
    EXPECT_EQ(2, theOctant->numBodies());
    theOctant->calculateCenterMass();
    EXPECT_EQ(glm::vec3(1, 1, 0), theOctant->getCenterMass()->getPosition());

    // Adding a third Body to the Octant and finding an non-integer center of mass
    theOctant->addBody(new Body(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 100));
    EXPECT_EQ(3, theOctant->numBodies());
    theOctant->calculateCenterMass();
    EXPECT_FLOAT_EQ(2.0f/3.0f, theOctant->getCenterMass()->getPosition().x);
    EXPECT_FLOAT_EQ(2.0f/3.0f, theOctant->getCenterMass()->getPosition().y);
    EXPECT_FLOAT_EQ(0, theOctant->getCenterMass()->getPosition().z);

    // Finding a center of mass when not all masses are the same
    theOctant->addBody(new Body(glm::vec3(0, 0, -1), glm::vec3(0, 0, 0), 500));
    theOctant->calculateCenterMass();
    EXPECT_FLOAT_EQ(1.0f/4.0f, theOctant->getCenterMass()->getPosition().x);
    EXPECT_FLOAT_EQ(1.0f/4.0f, theOctant->getCenterMass()->getPosition().y);
    EXPECT_FLOAT_EQ(-5.0f/8.0f, theOctant->getCenterMass()->getPosition().z);

    std::cout << "\n**** toString Output ****\n" << theOctant->toString() << "\n\n";*/
}

