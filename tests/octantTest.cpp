//
// Created by jackcamp on 1/5/19.
//

#include <gtest/gtest.h>
#include <exception>
#include "../src/n_body/octant.h"
#include "../src/n_body/body.h"


class octantTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        theOctant = new octant(glm::vec3(0,0,0), 16);
    }

    virtual void TearDown() {
        delete theOctant;
    }

    octant *theOctant;
};

TEST_F(octantTest, octantTest_addBody_Test) {

    // Adding one body to the octant
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

    cout << "\n**** toString Output ****\n" << theOctant->toString() << "\n\n";
}

TEST_F(octantTest, octantTest_getRelationships_Test) {

    body *body1 = new body(vec3(1, 1, 1), vec3(0, 0, 0), 100);


    // Adding several bodies
    theOctant->addBody(body1);
    theOctant->addBody(new body(vec3(1, 1, -1), vec3(0, 0, 0), 100));
    theOctant->addBody(new body(vec3(0, 0, 0), vec3(0, 0, 0), 100));
    theOctant->addBody(new body(vec3(0, 0, -1), vec3(0, 0, 0), 200));
    theOctant->addBody(new body(vec3(15, 15, 15), vec3(0, 0, 0), 100));
    theOctant->addBody(new body(vec3(15.1, 15, 15), vec3(0, 0, 0), 100));

    // Obtaining the relationships with the first body for theta = 1
    std::vector<relationship *> relationships = theOctant->getRelationships(body1, 1);

    cout << "\n\n";

    cout << std::to_string(relationships.size()) << " relationships created: \n\n";

    for (int i = 0; i < relationships.size(); ++i) {
        cout << "Relationship " << std::to_string(i) << "\n" << relationships[i]->toString() << "\n";
    }
}