//
// Created by jackcamp on 12/22/19.
//

#ifndef N_BODY_RUNNER_H
#define N_BODY_RUNNER_H

#include "model/calculation/Solver.h"
#include "model/calculation/BarnesHut/BarnesHutSolver.h"
#include "model/calculation/Naive/NaiveSolver.h"
#include "model/tools/BodyList.h"
#include "model/PhysicsContext.h"

#include <spdlog/spdlog.h>

namespace runner {

    int headless(BodyList bodies, PhysicsContext physicsContext, unsigned int cycles);

    // TODO
    int gui();
    int video();
};


#endif //N_BODY_RUNNER_H
