//
// Created by jackcamp on 2/20/22.
//

#include "ParticleEntry.h"


UI::ParticleEntry::ParticleEntry() : Gtk::Box(Gtk::Orientation::HORIZONTAL) {

    set_spacing(10);

    _grid.set_row_spacing(4);
    _grid.set_column_spacing(8);

    //_sphereView.set_expand();
    //append(_sphereView);

    _positionLabel.add_css_class("description");
    _velocityLabel.add_css_class("description");
    _massLabel.add_css_class("description");
    _grid.attach(_positionLabel, 0, 0);
    _grid.attach(_velocityLabel, 0, 1);
    _grid.attach(_massLabel, 2, 0);

    _grid.attach(_positionView, 1, 0);
    _grid.attach(_velocityView, 1, 1);
    _grid.attach(_massView, 3, 0);

    append(_grid);
}

void UI::ParticleEntry::bind(const NBody::Simulation::Particle &particle) {

    if (particle.all_of<NBody::Graphics::Color, NBody::Graphics::Sphere>()) {

        auto color = particle.get<NBody::Graphics::Color>();
        auto sphere = particle.get<NBody::Graphics::Sphere>();
        _sphereView.setValue(color, sphere);
    }

    auto position = particle.get<NBody::Physics::Position>();
    _positionView.setValue(position);

    auto velocity = particle.get<NBody::Physics::Velocity>();
    _velocityView.setValue(velocity);

    if (particle.all_of<NBody::Physics::ActiveMass>()) {
        _massLabel.show();
        _massView.show();
        auto mass = particle.get<NBody::Physics::ActiveMass>();
        _massView.set_text(std::to_string(mass.mass()));
    } else {
        _massLabel.hide();
        _massView.hide();
    }
}
