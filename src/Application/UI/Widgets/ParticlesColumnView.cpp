//
// Created by Jackson Campolattaro on 6/23/22.
//

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>

#include "ParticlesColumnView.h"


UI::ParticlesColumnView::ParticlesColumnView(NBody::Simulation &simulation) :
        Gtk::Box(Gtk::Orientation::VERTICAL),
        _simulation(simulation) {

    set_size_request(-1, 400);

    _particlesModel = ParticlesListModel::create(simulation);
    _selectionModel = Gtk::SingleSelection::create(_particlesModel);
    _columnView.set_model(_selectionModel);

    _columnView.append_column(Gtk::ColumnViewColumn::create("Active Mass",
                                                            BindableListItemFactory<ParticleActiveMassBindable>::create()));
    _columnView.append_column(Gtk::ColumnViewColumn::create("Passive Mass",
                                                            BindableListItemFactory<ParticlePassiveMassBindable>::create()));
    _columnView.append_column(
            Gtk::ColumnViewColumn::create("Position", BindableListItemFactory<ParticlePositionBindable>::create()));
    _columnView.append_column(
            Gtk::ColumnViewColumn::create("Velocity", BindableListItemFactory<ParticleVelocityBindable>::create()));

    _scrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    _scrolledWindow.set_vexpand();
    _scrolledWindow.set_child(_columnView);
    append(_scrolledWindow);

    _window.set_child(_particleEntry);

    // Show the particle window when the user selects a particle
    _selectionModel->signal_selection_changed().connect([&](guint p, guint n) {
        spdlog::debug("Selection changed, p={} n={}", p, n);
        if (auto selected = std::dynamic_pointer_cast<NBody::Simulation::Particle>(
                _selectionModel->get_selected_item())) {
            _particleEntry.bind(*selected);
            _window.set_title("Particle #" + std::to_string((ENTT_ID_TYPE) selected->entity()));
            _window.show();
        }
    });

    // When the user closes the window, hide it
    _window.signal_close_request().connect(
            [&] {
                _window.hide();
                _selectionModel->unselect_all();
                return true;
            },
            false
    );

}

