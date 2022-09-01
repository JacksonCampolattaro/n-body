//
// Created by Jackson Campolattaro on 8/24/22.
//

#include "ParticleGridCreatorWindow.h"

UI::ParticleGridCreatorWindow::ParticleGridCreatorWindow(NBody::Simulation &simulation) :
        _builder(Gtk::Builder::create_from_resource("/ui/particle_grid_creator_window.xml")),
        _particleGridCreator(*Gtk::Builder::get_widget_derived<ParticleGridCreator>(
                _builder, "grid-creator", simulation)) {

    set_title("Particle Grid Creator Wizard");
    set_child(_particleGridCreator);

    _particleGridCreator.signal_done.connect([&]() { hide(); });

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
