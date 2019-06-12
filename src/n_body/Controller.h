//
// Created by jackcamp on 5/21/19.
//

#ifndef N_BODY_POCCONTROLLER_H
#define N_BODY_CONTROLLER_H


#include "model/Model.h"
#include "view/View.h"
#include "view/Recorder.h"

#include <gtkmm.h>
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


    //signal accessor:
    typedef sigc::signal<void, int> type_signal_completed_frame;
    type_signal_completed_frame signal_completed_frame();

private:

    Model *model;

    View *view;

    vector<Body *> bodies;

    // Recording is optional
    Recorder *recorder = nullptr;


protected:

    type_signal_completed_frame m_signal_completed_frame;

};


#endif //N_BODY_POCCONTROLLER_H
