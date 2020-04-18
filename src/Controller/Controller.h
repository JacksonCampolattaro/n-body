//
// Created by jackcamp on 4/18/20.
//

#ifndef N_BODY_CONTROLLER_H
#define N_BODY_CONTROLLER_H

#include "Logger.h"

#include <string>

namespace Controller {

    class Controller {

    public:

        void openSimulation(const std::string& filepath);
        void saveSimulation(const std::string& filepath);
    };

}

#endif //N_BODY_CONTROLLER_H
