//
// Created by jackcamp on 10/25/18.
//

#include <gtest/gtest.h>
#include "../src/n_body/body.h"


int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(testBody, testBody_getMass_Test) {

    auto theBody = new body(vec3(0, 0, 0), vec3(0, 0, 0), 2, 2, vec3(1, 1, 1));

    //EXPECT_EQ(2, theBody->getMass());
}