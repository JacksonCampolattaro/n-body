//
// Created by jackcamp on 11/6/18.
//

#include <gtest/gtest.h>
#include "../src/n_body/model/Body.h"


class bodyTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        //theBody = new Body(glm::vec3(0, 0, 0));
    }

    virtual void TearDown() {
        //delete theBody;
    }

    //Body *theBody;
};

TEST_F(bodyTest, bodyTest_applyVelocity_Test) {

    auto theBody = new Body(glm::vec3(0, 0, 0));

    theBody->kick(glm::vec3(1, 0, 0));
    theBody->shiftBuffers();
    theBody->drift(1.0);
    theBody->shiftBuffers();
    EXPECT_EQ(glm::vec3(1, 0, 0), theBody->getVelocity());
    EXPECT_EQ(glm::vec3(1, 0, 0), theBody->getPosition());
}

/*

TEST_F(bodyTest, bodyTest_addVelocity_Test) {

    theBody->kick(glm::vec3(1, 0, 0));
    EXPECT_EQ(glm::vec3(2, 0, 0), theBody->getVelocity());
}

TEST_F(bodyTest, bodyTest_getPosition_Test) {

    EXPECT_EQ(glm::vec3(0, 0, 0), theBody->getPosition());
}

TEST_F(bodyTest, bodyTest_getVelocity_Test) {

    EXPECT_EQ(glm::vec3(1, 0, 0), theBody->getVelocity());
}

TEST_F(bodyTest, bodyTest_getMass_Test) {

    EXPECT_NEAR(2, theBody->getMass(), 0.1);
}

TEST_F(bodyTest, bodyTest_getRadius_Test) {

    //EXPECT_NEAR(0.781592607, theBody->getRadius(), 0.1);
}

TEST_F(bodyTest, bodyTest_getColor_Test) {

    //EXPECT_EQ(glm::vec3(1, 1, 1), theBody->getColor());
}*/
