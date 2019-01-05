//
// Created by jackcamp on 1/5/19.
//

#include <gtest/gtest.h>
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

    theOctant->addBody(new body(vec3(1, 1, 1), vec3(0, 0, 0), 100));
    EXPECT_EQ(vec3(1, 1, 1), theOctant->getCenterMass()->getPosition());
    EXPECT_EQ(100, theOctant->getCenterMass()->getMass());

    theOctant->addBody(new body(vec3(1, 1, 0), vec3(0, 0, 0), 100));
    //EXPECT_EQ(vec3(1, 1, 0.5), theOctant->getCenterMass()->getPosition());
}