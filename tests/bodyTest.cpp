//
// Created by jackcamp on 11/6/18.
//

#include <gtest/gtest.h>
#include "../src/n_body/body.h"


TEST(bodyTest, bodyTest_applyVelocity_Test) {

    auto theBody = new body(vec3(0, 0, 0), vec3(1, 0, 0), 18000, 1000, vec3(1, 1, 1));

    theBody->applyVelocity(1);
    EXPECT_EQ(vec3(1, 0, 0), theBody->getVelocity());
    EXPECT_EQ(vec3(1, 0, 0), theBody->getPosition());
}

TEST(bodyTest, bodyTest_applyForce_Test) {

    auto theBody = new body(vec3(0, 0, 0), vec3(0, 0, 0), 2, 1, vec3(1, 1, 1));

    theBody->applyForce(vec3(1, 0, 0), 2);
    EXPECT_EQ(vec3(1, 0, 0), theBody->getVelocity());
}

TEST(bodyTest, bodyTest_addVelocity_Test) {

    auto theBody = new body(vec3(0, 0, 0), vec3(0, 0, 0), 2, 1, vec3(1, 1, 1));

    theBody->addVelocity(vec3(1, 0, 0));
    EXPECT_EQ(vec3(1, 0, 0), theBody->getVelocity());
}

TEST(bodyTest, bodyTest_getPosition_Test) {

    auto theBody = new body(vec3(0, 0, 0), vec3(0, 0, 0), 2, 1, vec3(1, 1, 1));

    EXPECT_EQ(vec3(0, 0, 0), theBody->getPosition());
}

TEST(bodyTest, bodyTest_getVelocity_Test) {

    auto theBody = new body(vec3(0, 0, 0), vec3(2, 0, 0), 2, 1, vec3(1, 1, 1));

    EXPECT_EQ(vec3(2, 0, 0), theBody->getVelocity());
}

TEST(bodyTest, bodyTest_getMass_Test) {

    auto theBody = new body(vec3(0, 0, 0), vec3(0, 0, 0), 18000, 1000, vec3(1, 1, 1));

    EXPECT_NEAR(18000, theBody->getMass(), 0.1);
}

TEST(bodyTest, bodyTest_getRadius_Test) {

    auto theBody = new body(vec3(0, 0, 0), vec3(0, 0, 0), 10000, 1000, vec3(1, 1, 1));

    EXPECT_NEAR(1.33650, theBody->getRadius(), 0.1);
}

TEST(bodyTest, bodyTest_getColor_Test) {

    auto theBody = new body(vec3(0, 0, 0), vec3(3, 2, 1), 10000, 1000, vec3(1, 1, 1));

    EXPECT_EQ(vec3(1, 1, 1), theBody->getColor());
}
