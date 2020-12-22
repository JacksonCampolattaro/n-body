//
// Created by jackcamp on 12/21/20.
//

#ifndef N_BODY_INTERFACE_H
#define N_BODY_INTERFACE_H

#include <giomm.h>

namespace Interface {

    class Interface {
    public:

        sigc::signal<void()> signal_incrementSimulation;


    };
}

#endif //N_BODY_INTERFACE_H
