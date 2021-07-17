//
// Created by jackcamp on 7/15/21.
//

#ifndef N_BODY_SIMPLE_H
#define N_BODY_SIMPLE_H

#include "Interface.h"

#include <NBody/View/View.h>

namespace Interface {

    class Simple : public Interface {
    private:

        NBody::View _view{};

    public:

        Simple(Simulation &simulation) : Interface(simulation) {
            add(_view);
            _view.show();
        }
    };

}

#endif //N_BODY_SIMPLE_H
