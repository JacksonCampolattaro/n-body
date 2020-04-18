//
// Created by jackcamp on 4/18/20.
//

#ifndef N_BODY_CONTROLLER_H
#define N_BODY_CONTROLLER_H

#include "Logger.h"
#include "../Model/Simulation.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <string>
#include <fstream>

namespace Controller {

    class Controller {

    public:

        void loadSimulation(const std::string& filepath);
        void saveSimulation(const std::string& filepath);

    private:

        Model::Simulation _simulation;
    };

}

#endif //N_BODY_CONTROLLER_H
