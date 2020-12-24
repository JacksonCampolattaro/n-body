//
// Created by jackcamp on 12/21/20.
//

#ifndef N_BODY_INTERFACE_H
#define N_BODY_INTERFACE_H

#include <NBody/Simulation/EntityCollection.h>

#include <giomm.h>

namespace NBody::Interface {

    class Interface {
    public:

        sigc::signal<void()> signal_incrementSimulation;

        sigc::slot<void(const Simulation::EntityCollection &)> slot_setDrawables;


    };
}

#endif //N_BODY_INTERFACE_H
