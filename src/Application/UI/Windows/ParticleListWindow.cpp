//
// Created by Jackson Campolattaro on 8/24/22.
//

#include "ParticleListWindow.h"

UI::ParticleListWindow::ParticleListWindow(NBody::Simulation &simulation) : _particlesColumnView(simulation) {

    set_title("Particles");
    set_child(_particlesColumnView);

    signal_close_request().connect(
            [&] {
                hide();
                return true;
            },
            false
    );
}

sigc::signal<void(std::shared_ptr<NBody::Simulation::Particle> &)> &UI::ParticleListWindow::signal_openParticle() {
    return _particlesColumnView.signal_openParticle;
}

sigc::slot<void()> UI::ParticleListWindow::slot_open() {
    return sigc::mem_fun(*this, &Gtk::Window::show);
}
