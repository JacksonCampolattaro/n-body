//
// Created by jackcamp on 5/21/19.
//

#ifndef N_BODY_CONTROLLER_H
#define N_BODY_CONTROLLER_H


#include "model/Model.h"
#include "view/View.h"
#include "view/Recorder.h"

#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>

using std::vector;
using std::cout;
using std::endl;

class Controller {

public:

    Controller(Model *model, View *view, vector<Body *> bodies, Recorder *recorder = nullptr);

    void run();

private:

    Model *model;

    View *view;

    vector<Body *> bodies;

    // Recording is optional
    Recorder *recorder = nullptr;

};


#endif //N_BODY_CONTROLLER_H
