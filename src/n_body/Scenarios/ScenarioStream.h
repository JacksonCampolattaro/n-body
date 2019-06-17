//
// Created by jackcamp on 6/17/19.
//

#ifndef N_BODY_SCENARIOSTREAM_H
#define N_BODY_SCENARIOSTREAM_H

#include <cereal/archives/xml.hpp>
#include <iostream>
#include <fstream>
#include "../model/Body.h"

class ScenarioStream {

public:

    static void saveBodies(std::vector<Body> bodies, std::string filePath);
    static std::vector<Body> loadBodies(std::string filePath);


};


#endif //N_BODY_SCENARIOSTREAM_H
