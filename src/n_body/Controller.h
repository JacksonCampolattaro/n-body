//
// Created by jackcamp on 5/21/19.
//

#ifndef N_BODY_POCCONTROLLER_H
#define N_BODY_CONTROLLER_H


#include "model/calculation/Solver.h"

#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>

using std::vector;
using std::cout;
using std::endl;

class Controller {

public:

    Controller(vector<Body> *bodies, PhysicsContext *physics, Solver *solver);

    void run();

    void increment();

    void on_preparing_solver();

    void on_solving();

    void on_shifting_buffers();

    void on_solver_complete();

private:

    Solver *solver;
    PhysicsContext *physics;
    vector<Body> *bodies;


    int numFrames = 255000;
    int frameNum = 0;

};


#endif //N_BODY_POCCONTROLLER_H
