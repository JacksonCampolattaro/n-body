//
// Created by jackcamp on 2/20/22.
//

#include "ParticleEntry.h"


UI::ParticleEntry::ParticleEntry() : Gtk::Grid() {

    _icon.set(_pixbuf); //set_from_resource("/NBody/icons/node_material.svg");
    _icon.set_expand();
    attach(_icon, 0, 0, 1, 2);

    attach(_positionEntry, 1, 0);
    attach(_velocityLabel, 1, 1);

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
    _positionEntry.on_update(position.x, position.y, position.z);
    _positionEntry.on_valueChanged();
}
