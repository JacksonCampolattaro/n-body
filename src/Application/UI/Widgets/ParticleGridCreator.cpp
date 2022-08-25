//
// Created by Jackson Campolattaro on 8/24/22.
//

#include "ParticleGridCreator.h"

#include <giomm/simpleactiongroup.h>

UI::ParticleGridCreator::ParticleGridCreator(NBody::Simulation &simulation) :
        BuilderWidget("/ui/particle_grid_creator.xml"),
        _gridCornerEntry(getWidget<CompactPositionEntry>("grid-corner-entry")),
        _spacingEntry(getWidget<FloatEntry>("spacing-entry")),
        _massEntry(getWidget<FloatEntry>("mass-entry")),
        _velocityEntry(getWidget<CompactVelocityEntry>("velocity-entry")),
        _activeEntry(getWidget<Gtk::CheckButton>("active-entry")),
        _passiveEntry(getWidget<Gtk::CheckButton>("passive-entry")),
        _colorEntry(getWidget<Gtk::ColorButton>("color-entry")),
        _radiusEntry(getWidget<FloatEntry>("radius-entry"))
        {

            auto actionGroup = Gio::SimpleActionGroup::create();
            insert_action_group("grid-creator", actionGroup);

            actionGroup->add_action("create",
                                    [&]() {
                                        spdlog::debug("Creating a grid of particles");
                                    });
}
