//
// Created by jackcamp on 2/20/22.
//
#include "ParticleEntry.h"
#include "FloatEntry.h"
#include "VectorEntry.h"


UI::ParticleEntry::ParticleEntry() :
        Gtk::Box(Gtk::Orientation::HORIZONTAL),
        _builder(Gtk::Builder::create_from_resource("/ui/particle_entry.xml")),
        _positionEntry(*Gtk::Builder::get_widget_derived<ParticlePositionEntry>(_builder, "position-entry")),
        _velocityEntry(*Gtk::Builder::get_widget_derived<ParticleVelocityEntry>(_builder, "velocity-entry")),
        _activeMassEntry(*Gtk::Builder::get_widget_derived<ParticleActiveMassEntry>(_builder, "active-mass-entry")),
        _passiveMassEntry(*Gtk::Builder::get_widget_derived<ParticlePassiveMassEntry>(_builder, "passive-mass-entry")),
        _colorEntry(*Gtk::Builder::get_widget_derived<ParticleColorEntry>(_builder, "color-entry")),
        _radiusEntry(*Gtk::Builder::get_widget_derived<ParticleRadiusEntry>(_builder, "radius-entry")) {

    auto root = _builder->get_widget<Gtk::Widget>("root");
    append(*root);
}

void UI::ParticleEntry::bind(std::shared_ptr<NBody::Simulation::Particle> &particle) {

    _positionEntry.bind(particle);
    _velocityEntry.bind(particle);

    if (particle->all_of<NBody::Physics::ActiveMass>()) {
        _activeMassEntry.bind(particle);
    } else {
        _activeMassEntry.unbind();
    }

    if (particle->all_of<NBody::Physics::PassiveMass>()) {
        _passiveMassEntry.bind(particle);
    } else {
        _passiveMassEntry.unbind();
    }

    if (particle->all_of<NBody::Graphics::Color, NBody::Graphics::Sphere>()) {
        _colorEntry.bind(particle);
        _radiusEntry.bind(particle);
    } else {
        _colorEntry.unbind();
        _radiusEntry.unbind();
    }
}
