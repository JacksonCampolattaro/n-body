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

TEST(bodyTest, bodyTest_getMass_Test) {

    auto theBody = new body(vec3(0, 0, 0), vec3(0, 0, 0), 18000, 1000, vec3(1, 1, 1));

    EXPECT_EQ(18000, theBody->getMass());
}
