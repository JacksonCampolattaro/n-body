//
// Created by jackcamp on 2/16/22.
//

#include "ParticlesPanel.h"

UI::ParticlesPanel::ParticlesPanel(NBody::Simulation &simulation) :
        Panel("Particles"),
        _list(),
        _particlesView(simulation),
        _countWidget("Count", "The number of particles", "#"),
        _example("Example", "Demonstration of how a switch works") {

    _list.set_hexpand();
    _list.get_style_context()->add_class("rich-list");
    append(_list);

    _list.append(_countWidget);
    _list.append(_example);

    _countWidget.widget().set_text(std::to_string(simulation.size()));

    append(_particlesView);
}
