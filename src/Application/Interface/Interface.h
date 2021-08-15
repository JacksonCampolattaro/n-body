#ifndef N_BODY_INTERFACE_H
#define N_BODY_INTERFACE_H

#include <gtkmm/window.h>

#include <NBody/Simulation/Simulation.h>

namespace Interface {

    using NBody::Simulation;

    class Interface : public Gtk::Window {
    private:

        Simulation &_simulation;

    public:

        Interface(Simulation &simulation) : _simulation(simulation) {}

    public:
        // Signals

    public:
        // Slots

        // TODO what responsibilities will the view have?
    };
}


#endif //N_BODY_INTERFACE_H
