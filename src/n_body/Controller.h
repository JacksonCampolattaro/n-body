//
// Created by jackcamp on 5/21/19.
//

#ifndef N_BODY_POCCONTROLLER_H
#define N_BODY_CONTROLLER_H


#include "view/View.h"
#include "view/Recorder.h"
#include "model/calculation/Solver.h"

#include <gtkmm.h>
#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>

using std::vector;
using std::cout;
using std::endl;

class Controller {

public:

    Controller(vector<Body> *bodies, PhysicsContext *physics, Solver *solver, View *view = nullptr, Recorder *recorder = nullptr);

    void run();

    void increment();

    void on_preparing_solver();

    void on_solving();

    void on_shifting_buffers();

    void on_solver_complete();

private:

    Solver *solver;
    PhysicsContext *physics;
    View *view;
    vector<Body> *bodies;
    // Recording is optional
    Recorder *recorder = nullptr;


    int numFrames = 255;
    int frameNum = 0;

};


#endif //N_BODY_POCCONTROLLER_H
