//
// Created by jackcamp on 7/15/21.
//

#ifndef N_BODY_SIMPLE_H
#define N_BODY_SIMPLE_H

#include "Interface.h"

#include <NBody/View/View.h>
#include <NBody/View/ArcBallCamera.h>

namespace Interface {

    class Simple : public Interface {
    private:

        Magnum::Examples::ArcBallCamera _camera;
        NBody::View _view;

    public:

        Simple(Simulation &simulation);
    };

}

#endif //N_BODY_SIMPLE_H
