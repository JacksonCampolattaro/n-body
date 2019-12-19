//
// Created by jackcamp on 10/25/18.
//

#include "model/tools/BodyList.h"
#include "model/PhysicsContext.h"

#include <fstream>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>

int main(int argc, char **argv) {

    // Loading from an xml file
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Loading Bodies
    std::ifstream bodiesInputStream("/home/jackcamp/CLionProjects/n_body/scenarios/test/threeBody.bod");
    if (!bodiesInputStream.is_open())
        return 1;
    cereal::XMLInputArchive bodyArchive(bodiesInputStream);
    auto bodies = BodyList();
    bodyArchive(bodies);

    // Loading Physics
    std::ifstream physicsInputStream("/home/jackcamp/CLionProjects/n_body/scenarios/test/threeBody.phys");
    if (!bodiesInputStream.is_open())
        return 2;
    cereal::XMLInputArchive physicsArchive(physicsInputStream);
    auto physics = PhysicsContext();
    physicsArchive(physics);


    // Creating a simulation from the loaded data
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Running the simulation for a number of cycles
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Saving the result to an xml file
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Saving Physics

    // Saving Bodies


}