//
// Created by jackcamp on 5/22/19.
//

#include "Preset.h"


void Preset::cubicGrid(Position cornerPosition, Velocity velocity,
                       glm::vec3 size, float spacing, float mass) {

    float density = 40;

    // Creates a cubic grid

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            for (int z = 0; z < size.z; ++z) {

                glm::vec3 coordinate(x, y, z);
                Position position = Position(cornerPosition + (coordinate * spacing));

                if (x < 0) {
                    velocity *= -1;
                }

                // rgbaColor options
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

                glm::vec3 color = blueToGreen;
                auto newBody = new Body(position);
                newBody->setVelocity(velocity)->setMass(mass)->setDensity(40)->setColor(rgbaColor(color.r, color.g, color.b, 1.0));
                bodies.push_back(newBody);
            }
        }
    }
}

void Preset::blender() {

    physicsContext->setT(0.005)->setG(0.01)->setPower(2);

    // Massive fixed mass
    auto superHeavy = new Body(Position(-60, 0, -500));
    superHeavy->setMass(180000000)->setColor(orange)->setDensity(10000)->setVelocity(Velocity(0, -90, 0));
    bodies.push_back(superHeavy);

    auto superHeavy2 = new Body(Position(60, 0, -500));
    superHeavy2->setMass(180000000)->setColor(orange)->setDensity(10000)->setVelocity(Velocity(0, 90, 0));
    bodies.push_back(superHeavy2);

    // Cubic Grid
    Position cornerPosition(-50, -100, -450);
    Velocity velocity(60.0f, 20.0f, 0.0f);
    glm::vec3 size(6, 6, 80);
    float spacing = 10.0f;
    float mass = 5000.0f;

    cubicGrid(cornerPosition, velocity, size, spacing, mass);
}

void Preset::bigDemo() {

    physicsContext->setT(0.001)->setG(0.01)->setPower(2);

    // Massive fixed mass
    auto superHeavy = new Body(Position(0, 0, -500));
    superHeavy->setMass(190000000)->setColor(white)->setDensity(10000)->makeFixed();
    bodies.push_back(superHeavy);

    // Cubic Grid
    Position cornerPosition(-50, -150, -400);
    Velocity velocity(60.0f, 20.0f, 0.0f);
    glm::vec3 size(10, 10, 20);
    float spacing = 10.0f;
    float mass = 5000.0f;

    cubicGrid(cornerPosition, velocity, size, spacing, mass);
}

void Preset::threeBodyDemo() {

    physicsContext->setPower(1)->setG(.2)->setT(0.01);
    float density = 40;

    auto redBody = new Body(Position(20.0, 50.0, -100));
    redBody->setVelocity(Velocity(30, -50, 0))->setMass(500000)->setDensity(density)->setColor(red);
    bodies.push_back(redBody);
    auto whiteBody = new Body(Position(0.0, -50, -200));
    whiteBody->setVelocity(Velocity(-30, 0, 0))->setMass(500000)->setDensity(density)->setColor(white);
    bodies.push_back(whiteBody);
    auto yellowBody = new Body(Position(-20, 0, -500));
    yellowBody->setVelocity(Velocity(0, 50, 0))->setMass(500000)->setDensity(density)->setColor(yellow);
    bodies.push_back(yellowBody);
}