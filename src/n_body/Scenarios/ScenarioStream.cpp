//
// Created by jackcamp on 6/17/19.
//

#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include "ScenarioStream.h"

void ScenarioStream::saveBodies(std::vector<Body> bodies, std::string filePath) {

    // Creates an archive at the file path location
    std::ofstream file(filePath);
    cereal::XMLOutputArchive outputArchive(file);

    // Passes the bodies into the archive
    /*for (int i = 0; i < bodies.size(); ++i) {
        outputArchive(cereal::make_nvp("body" + std::to_string(i),*bodies[i]));
    }*/
    outputArchive(CEREAL_NVP(bodies));
}

std::vector<Body> ScenarioStream::loadBodies(std::string filePath) {

    std::ifstream file(filePath);
    cereal::XMLInputArchive inputArchive(file);

    std::vector<Body> bodyList;
    inputArchive(bodyList);

    return bodyList;
}
