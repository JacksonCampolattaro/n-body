//
// Created by Jackson Campolattaro on 8/24/22.
//

#include "ParticleGridCreator.h"

#include <giomm/simpleactiongroup.h>

UI::ParticleGridCreator::ParticleGridCreator(Gtk::Box::BaseObjectType *cobject,
                                             const Glib::RefPtr<Gtk::Builder> &builder,
                                             NBody::Simulation &simulation) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/particle_grid_creator.xml"),
        _simulation(simulation),
        _gridCornerEntry(getWidget<CompactPositionEntry>("grid-corner-entry")),
        _gridXSizeEntry(getWidget<SimplePositiveSizeEntry>("grid-x-size-entry")),
        _gridYSizeEntry(getWidget<SimplePositiveSizeEntry>("grid-y-size-entry")),
        _gridZSizeEntry(getWidget<SimplePositiveSizeEntry>("grid-z-size-entry")),
        _spacingEntry(getWidget<FloatEntry>("spacing-entry")),
        _velocityEntry(getWidget<CompactVelocityEntry>("velocity-entry")),
        _massEntry(getWidget<FloatEntry>("mass-entry")),
        _accelerationEntry(getWidget<Gtk::CheckButton>("acceleration-entry")),
        _colorEntry(getWidget<Gtk::ColorButton>("color-entry")),
        _radiusEntry(getWidget<FloatEntry>("radius-entry")) {

    auto actionGroup = Gio::SimpleActionGroup::create();
    insert_action_group("grid-creator", actionGroup);
    actionGroup->add_action("create",
                            sigc::mem_fun(*this, &ParticleGridCreator::createGrid));

    // Set some reasonable defaults
    _gridXSizeEntry.setValue(10);
    _gridYSizeEntry.setValue(10);
    _gridZSizeEntry.setValue(10);
    _spacingEntry.setValue(10.0f);
    _massEntry.setValue(1.0f);
    _accelerationEntry.set_active();
    _colorEntry.set_rgba(Gdk::RGBA("lightblue"));
    _radiusEntry.setValue(1.0f);
}

void UI::ParticleGridCreator::createGrid() {

    auto cornerPosition = _gridCornerEntry.getValue();
    auto xSize = _gridXSizeEntry.getValue();
    auto ySize = _gridYSizeEntry.getValue();
    auto zSize = _gridZSizeEntry.getValue();
    auto spacing = _spacingEntry.get_value();

    auto mass = _massEntry.get_value();
    auto velocity = _velocityEntry.getValue();
    auto acceleration = _accelerationEntry.get_active();

    auto color = _colorEntry.get_rgba();
    auto radius = _radiusEntry.get_value();

    spdlog::debug("Generating a {}x{}x{} grid of entities", xSize, ySize, zSize);

    std::vector<NBody::Entity> entities{xSize * ySize * zSize};
    _simulation.create(entities.begin(), entities.end());

    auto currentEntity = entities.begin();
    for (int x = 0; x < xSize; ++x) {
        for (int y = 0; y < ySize; ++y) {
            for (int z = 0; z < zSize; ++z) {

                NBody::Simulation::Particle particle = {_simulation, *(currentEntity++)};

                auto offset = glm::vec3{spacing * x, spacing * y, spacing * z};
                particle.setPosition(cornerPosition + offset);
                if (mass > 0.0f) particle.setMass((float) mass);
                particle.setVelocity(velocity);
                if (acceleration) particle.setAcceleration({0.0f, 0.0f, 0.0f});
                particle.setColor({color.get_red(), color.get_green(), color.get_blue()});
                particle.setSphere({(float) radius});
            }
        }
    }

    _simulation.signal_particles_added.emit(entities);
    _simulation.signal_changed.emit();
    signal_done.emit();
}
