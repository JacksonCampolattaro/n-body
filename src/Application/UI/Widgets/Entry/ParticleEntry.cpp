//
// Created by jackcamp on 2/20/22.
//
#include "ParticleEntry.h"
#include "FloatEntry.h"
#include "VectorEntry.h"


UI::ParticleEntry::ParticleEntry() :
        BuilderWidget<Gtk::Box>("/ui/particle_entry.xml"),
        _positionEntry(getWidget<ParticlePositionEntry>("position-entry")),
        _velocityEntry(getWidget<ParticleVelocityEntry>("velocity-entry")),
        _massEntry(getWidget<ParticleMassEntry>("mass-entry")),
        _activeEntry(getWidget<ParticleActiveTagEntry>("active-entry")),
        _passiveEntry(getWidget<ParticlePassiveTagEntry>("passive-entry")),
        _colorEntry(getWidget<ParticleColorEntry>("color-entry")),
        _radiusEntry(getWidget<ParticleRadiusEntry>("radius-entry")) {}

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
