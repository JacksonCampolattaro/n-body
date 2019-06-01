//
// Created by jackcamp on 5/22/19.
//

#include "Preset.h"


void Preset::cubicGrid(glm::vec3 cornerPosition, glm::vec3 velocity,
               glm::vec3 size, float spacing, float mass) {

    float density = 40;

    // Creates a cubic grid

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            for (int z = 0; z < size.z; ++z) {

                glm::vec3 coordinate(x, y, z);
                glm::vec3 position = cornerPosition + (coordinate * spacing);

                if (x < 0) {
                    velocity *= -1;
                }

                // Color options
                glm::vec3 multiColor = coordinate / size + 0.2f;
                glm::vec3 yellowToRed(1, coordinate.y / size.y, 0);
                glm::vec3 yellowToRed_Z(1, coordinate.z / size.z, 0);
                glm::vec3 greenToWhite(coordinate.y / size.y, 1, coordinate.y / size.y);
                glm::vec3 blueToRed(coordinate.y / size.y, 0, 1 - coordinate.y / size.y);
                glm::vec3 blueToRed_Z(coordinate.z / size.z, 0, 1 - coordinate.z / size.z);
                glm::vec3 blueToRedToWhite(coordinate.y / size.y + coordinate.x / size.x, coordinate.x / size.x,
                                           1 - coordinate.y / size.y + coordinate.x / size.x);
                glm::vec3 whiteToBlueToRed(coordinate.y / size.y + coordinate.z / size.z, coordinate.z / size.z,
                                           1 - coordinate.y / size.y + coordinate.z / size.z);
                glm::vec3 blueToGreen(0, 1 - coordinate.z / size.z, coordinate.z / size.z);
                glm::vec3 rainbow(coordinate.y / size.y, coordinate.x / size.x, 1 - coordinate.y / size.y);


                auto newBody = new Body(position);
                newBody->setVelocity(velocity)->setMass(mass)->setDensity(40)->setColor(whiteToBlueToRed);
                bodies.push_back(newBody);
            }
        }
    }
}

void Preset::blender() {

    physicsContext->setT(0.005)->setG(0.01)->setPower(2);

    // Massive fixed mass
    auto superHeavy = new Body(glm::vec3(-60, 0, -500));
    superHeavy->setMass(180000000)->setColor(orange)->setDensity(10000)->setVelocity(glm::vec3(0, -90, 0));
    bodies.push_back(superHeavy);

    auto superHeavy2 = new Body(glm::vec3(60, 0, -500));
    superHeavy2->setMass(180000000)->setColor(orange)->setDensity(10000)->setVelocity(glm::vec3(0, 90, 0));
    bodies.push_back(superHeavy2);

    // Cubic Grid
    glm::vec3 cornerPosition(-50, -100, -450);
    glm::vec3 velocity(60.0f, 20.0f, 0.0f);
    glm::vec3 size(6, 6, 80);
    float spacing = 10.0f;
    float mass = 5000.0f;

    cubicGrid(cornerPosition, velocity, size, spacing, mass);
}

void Preset::bigDemo() {

    physicsContext->setT(0.001)->setG(0.01)->setPower(2);

    // Massive fixed mass
    auto superHeavy = new Body(glm::vec3(0, 0, -500));
    superHeavy->setMass(190000000)->setColor(white)->setDensity(10000)->makeFixed();
    bodies.push_back(superHeavy);

    // Cubic Grid
    glm::vec3 cornerPosition(-50, -150, -400);
    glm::vec3 velocity(60.0f, 20.0f, 0.0f);
    glm::vec3 size(10, 10, 20);
    float spacing = 10.0f;
    float mass = 5000.0f;

    cubicGrid(cornerPosition, velocity, size, spacing, mass);
}

void Preset::threeBodyDemo() {

    physicsContext->setPower(1)->setG(.2);
    float density = 40;

    auto redBody = new Body(glm::vec3(20.0, 50.0, -300));
    redBody->setVelocity(glm::vec3(30, -50, 0))->setMass(500000)->setDensity(density)->setColor(red);
    bodies.push_back(redBody);
    auto whiteBody = new Body(glm::vec3(0.0, -50, -400));
    whiteBody->setVelocity(glm::vec3(-30, 0, 0))->setMass(500000)->setDensity(density)->setColor(white);
    bodies.push_back(whiteBody);
    auto yellowBody = new Body(glm::vec3(-20, 0, -700));
    yellowBody->setVelocity(glm::vec3(0, 50, 0))->setMass(500000)->setDensity(density)->setColor(yellow);
    bodies.push_back(yellowBody);
}