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
        _massCheckButton(getWidget<ParticleMassCheckButton>("mass-checkbutton")),
        _massEntry(getWidget<ParticleMassEntry>("mass-entry")),
        _accelerationCheckButton(getWidget<ParticleAccelerationCheckButton>("acceleration-checkbutton")),
        _accelerationEntry(getWidget<ParticleAccelerationEntry>("acceleration-entry")),
        _colorEntry(getWidget<ParticleColorEntry>("color-entry")),
        _radiusEntry(getWidget<ParticleRadiusEntry>("radius-entry")) {}

void UI::ParticleEntry::bind(std::shared_ptr<NBody::Simulation::Particle> &particle) {
    _particle = particle;

    _positionEntry.bind(particle);
    _velocityEntry.bind(particle);

    _massCheckButton.bind(particle);
    _massEntry.bind(particle);

    _accelerationCheckButton.bind(particle);
    _accelerationEntry.bind(particle);

    _colorEntry.bind(particle);
    _radiusEntry.bind(particle);

//    _physicsCheckButton.set_active(particle->all_of<NBody::Physics::Mass>());
//    _physicsCheckButton.signal_toggled().connect([&]() {
//
//        if (_physicsCheckButton.get_active()) {
//            _particle->setMass((float) _massEntry.get_value());
//            _activeEntry.set_active(true);
//            _passiveEntry.set_active(true);
//        } else {
//            _particle->remove<NBody::Physics::Mass>();
//            _particle->remove<NBody::Physics::ActiveTag>();
//            _particle->remove<NBody::Physics::PassiveTag>();
//            _activeEntry.set_active(false);
//            _passiveEntry.set_active(false);
//        }
//        _particle->get<sigc::signal<void()>>().emit();
//    });
//
//    _appearanceCheckButton.set_active(particle->all_of<NBody::Graphics::Color, NBody::Graphics::Sphere>());
//    _appearanceCheckButton.signal_toggled().connect([&]() {
//
//        if (_appearanceCheckButton.get_active()) {
//            auto color = _colorEntry.get_rgba();
//            _particle->setColor({color.get_red(), color.get_green(), color.get_blue()});
//            _particle->setSphere({1.0f});
//        } else {
//            _particle->remove<NBody::Graphics::Color>();
//            _particle->remove<NBody::Graphics::Sphere>();
//        }
//        _particle->get<sigc::signal<void()>>().emit();
//    });
}

std::shared_ptr<NBody::Simulation::Particle> UI::ParticleEntry::currentParticle() {
    return _particle;
}
