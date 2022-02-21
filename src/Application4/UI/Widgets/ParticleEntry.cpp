//
// Created by jackcamp on 2/20/22.
//

#include "ParticleEntry.h"


UI::ParticleEntry::ParticleEntry() : Gtk::Grid() {

    set_row_spacing(4);
    set_column_spacing(8);

    _icon.set(Gdk::Pixbuf::create_from_resource("/NBody/icons/node_material.svg", -1, 256));
    _icon.set_expand();
    attach(_icon, 0, 0, 1, 2);

    _positionLabel.add_css_class("description");
    _velocityLabel.add_css_class("description");
    _massLabel.add_css_class("description");
    attach(_positionLabel, 1, 0);
    attach(_velocityLabel, 1, 1);
    attach(_massLabel, 3, 0);

    attach(_positionView, 2, 0);
    attach(_velocityView, 2, 1);
    attach(_massView, 4, 0);


//    append(_positionLabel);
//
//    append(_positionEntry);
//
//    append(_velocityLabel);
//
//    append(_massLabel);
}

void UI::ParticleEntry::bind(const NBody::Simulation::Particle &particle) {

    auto position = particle.get<NBody::Physics::Position>();
}
