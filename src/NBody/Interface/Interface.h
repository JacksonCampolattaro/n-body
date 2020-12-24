//
// Created by jackcamp on 12/21/20.
//

#ifndef N_BODY_INTERFACE_H
#define N_BODY_INTERFACE_H

#include <NBody/Simulation/Simulation.h>

#include <giomm.h>

namespace NBody::Interface {

    class Interface {
    public:

        sigc::signal<void()> signal_incrementSimulation;

        sigc::slot<void(const Simulation::Simulation &)> slot_setSimulation;


    };
}

#endif //N_BODY_INTERFACE_H
