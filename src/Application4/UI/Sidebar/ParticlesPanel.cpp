//
// Created by jackcamp on 2/16/22.
//

#include "ParticlesPanel.h"

UI::ParticlesPanel::ParticlesPanel(NBody::Simulation &simulation) :
        Panel("Particles"),
        _particlesView(simulation) {

    auto builder = Gtk::Builder::create_from_resource("/ui/particles_panel.xml");
    auto root = builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);

    auto count = builder->get_widget<Gtk::Label>("count-label");
    count->set_text(std::to_string(simulation.size()));

    //append(_particlesView);
}
