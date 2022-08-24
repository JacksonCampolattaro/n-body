//
// Created by Jackson Campolattaro on 8/24/22.
//

#include "ParticleGridCreatorWindow.h"

UI::ParticleGridCreatorWindow::ParticleGridCreatorWindow(NBody::Simulation &simulation) :
        _particleGridCreator(simulation) {


    set_title("Particle Grid Creator Wizard");
    set_child(_particleGridCreator);

    signal_close_request().connect(
            [&] {
                hide();
                return true;
            },
            false
    );
}

sigc::slot<void()> UI::ParticleGridCreatorWindow::slot_open() {
    return sigc::mem_fun(*this, &Gtk::Window::show);
}
