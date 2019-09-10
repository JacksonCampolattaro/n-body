//
// Created by jackcamp on 9/9/19.
//

#ifndef N_BODY_LAUNCHER_H
#define N_BODY_LAUNCHER_H


#include <QtWidgets>
#include "ViewportWidget.h"

using std::vector;

class Launcher : public QWidget {

public:

    Launcher();

    void start();

    void next();

private:

    ViewportWidget *viewport;

    Solver *solver;
    PhysicsContext *physics;
    vector<Body> *bodies;

    Controller *controller;
};


#endif //N_BODY_LAUNCHER_H
