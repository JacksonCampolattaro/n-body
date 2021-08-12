//
// Created by jackcamp on 8/12/21.
//

#ifndef N_BODY_INTERACTIVE_H
#define N_BODY_INTERACTIVE_H

#include <NBody/Simulation/Simulation.h>
#include <NBody/View/View.h>
#include <gtkmm/entry.h>

#include "Interface.h"

namespace Interface {

    using NBody::Simulation::Simulation;

    class Interactive : public Interface {
    private:

        NBody::View _view;
        Gtk::Entry _entry;

    public:

        Interactive(Simulation &simulation) : Interface(simulation), _view(simulation) {
            add(_view);
            _view.show();

            signal_key_press_event().connect(sigc::mem_fun(*this, &Interactive::on_keyPressEvent), false);
        }

    public:
        // Signals

    public:
        // Slots

        bool on_keyPressEvent(GdkEventKey* event) {

            if (event->keyval == GDK_KEY_Left) {
                std::cout << "←" << std::endl;
            }

            if (event->keyval == GDK_KEY_Right) {
                std::cout << "→" << std::endl;
            }

            if (event->keyval == GDK_KEY_Up) {
                std::cout << "↑" << std::endl;
            }

            if (event->keyval == GDK_KEY_Down) {
                std::cout << "↓" << std::endl;
            }

            return false;
        }

        // TODO what responsibilities will the view have?
    };
}


#endif //N_BODY_INTERACTIVE_H
