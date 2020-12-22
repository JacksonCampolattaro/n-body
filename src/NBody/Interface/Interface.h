//
// Created by jackcamp on 12/21/20.
//

#ifndef N_BODY_INTERFACE_H
#define N_BODY_INTERFACE_H

#include <NBody/Model/Drawable/Drawable.h>

#include <giomm.h>

using namespace Model;

namespace NBody::Interface {

    class Interface {
    public:

        sigc::signal<void()> signal_incrementSimulation;

        sigc::slot<void(const std::deque<Drawable::Drawable> *drawables)> slot_setDrawables;


    };
}

#endif //N_BODY_INTERFACE_H
