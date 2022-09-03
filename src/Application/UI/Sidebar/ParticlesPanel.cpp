//
// Created by jackcamp on 2/16/22.
//

#include <spdlog/spdlog.h>
#include <gtkmm/applicationwindow.h>
#include <fstream>
#include <giomm/simpleactiongroup.h>

#include "ParticlesPanel.h"

UI::ParticlesPanel::ParticlesPanel(Gtk::Box::BaseObjectType *cobject,
                                   const Glib::RefPtr<Gtk::Builder> &builder,
                                   NBody::Simulation &simulation) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/particles_panel.xml"),
        _simulation(simulation),
        _modifyButton(getWidget<Gtk::Button>("modify-button")),
        _countLabel(getWidget<Gtk::Label>("count-label")),
        _averagePositionView(getWidget<PositionView>("average-position-view")),
        _totalMassView(getWidget<PreciseFloatView>("total-mass-label")),
        _centerOfMassPositionView(getWidget<PositionView>("center-of-mass-position-view")),
        _interactionCountLabel(getWidget<Gtk::Label>("interaction-count-label")) {

    _simulation.signal_changed.connect([&]() {

        _countLabel.set_text(std::to_string(simulation.particleCount()));
        _averagePositionView.setValue(simulation.averagePosition());

        _totalMassView.setValue(simulation.totalMass());
        _centerOfMassPositionView.setValue(simulation.centerOfMass());

        _interactionCountLabel.set_text(std::to_string(simulation.interactionCount()));
    });
    _simulation.signal_changed.emit();
}

Glib::SignalProxy<void()> UI::ParticlesPanel::signal_openList() {
    return _modifyButton.signal_clicked();
}
