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
        _massEntry(*Gtk::Builder::get_widget_derived<ParticleMassEntry>(_builder, "mass-entry")),
        _activeEntry(*Gtk::Builder::get_widget_derived<ParticleActiveTagEntry>(_builder, "active-entry")),
        _passiveEntry(*Gtk::Builder::get_widget_derived<ParticlePassiveTagEntry>(_builder, "passive-entry")),
        _colorEntry(*Gtk::Builder::get_widget_derived<ParticleColorEntry>(_builder, "color-entry")),
        _radiusEntry(*Gtk::Builder::get_widget_derived<ParticleRadiusEntry>(_builder, "radius-entry")) {

    auto root = _builder->get_widget<Gtk::Widget>("root");
    append(*root);
}

void UI::ParticleEntry::bind(std::shared_ptr<NBody::Simulation::Particle> &particle) {

    _positionEntry.bind(particle);
    _velocityEntry.bind(particle);

    if (particle->all_of<NBody::Physics::Mass>()) {
        _massEntry.bind(particle);
    } else {
        _massEntry.unbind();
    }

    _activeEntry.bind(particle);
    _passiveEntry.bind(particle);

    if (particle->all_of<NBody::Graphics::Color, NBody::Graphics::Sphere>()) {
        _colorEntry.bind(particle);
        _radiusEntry.bind(particle);
    } else {
        _colorEntry.unbind();
        _radiusEntry.unbind();
    }
}
