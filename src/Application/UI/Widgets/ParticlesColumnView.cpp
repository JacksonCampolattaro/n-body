//
// Created by Jackson Campolattaro on 6/23/22.
//

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>
#include <gtkmm/eventcontrollerkey.h>

#include "ParticlesColumnView.h"


UI::ParticlesColumnView::ParticlesColumnView(NBody::Simulation &simulation) :
        Gtk::Box(Gtk::Orientation::VERTICAL),
        _simulation(simulation) {

    set_size_request(-1, 400);

    _particlesModel = ParticlesListModel::create(simulation);
    _selectionModel = Gtk::SingleSelection::create(_particlesModel);
    _columnView.set_model(_selectionModel);

    _columnView.append_column(Gtk::ColumnViewColumn::create(
            "ID",
            BindableListItemFactory<ParticleIDView>::create()
    ));
    _columnView.append_column(Gtk::ColumnViewColumn::create(
            "Appearance",
            BindableListItemFactory<ParticleIconView>::create()
    ));
    _columnView.append_column(Gtk::ColumnViewColumn::create(
            "Mass",
            BindableListItemFactory<ParticleMassView>::create()
    ));
    _columnView.append_column(Gtk::ColumnViewColumn::create(
            "Active",
            BindableListItemFactory<ParticleActiveTagView>::create()
    ));
    _columnView.append_column(Gtk::ColumnViewColumn::create(
            "Passive",
            BindableListItemFactory<ParticlePassiveTagView>::create()
    ));
    _columnView.append_column(Gtk::ColumnViewColumn::create(
            "Position",
            BindableListItemFactory<ParticlePositionView>::create()
    ));
    _columnView.append_column(Gtk::ColumnViewColumn::create(
            "Velocity",
            BindableListItemFactory<ParticleVelocityView>::create()
    ));
    _columnView.append_column(Gtk::ColumnViewColumn::create(
            "Color",
            BindableListItemFactory<ParticleColorView>::create()
    ));
    _columnView.append_column(Gtk::ColumnViewColumn::create(
            "Radius",
            BindableListItemFactory<ParticleRadiusView>::create()
    ));

    _scrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    _scrolledWindow.set_vexpand();
    _scrolledWindow.set_child(_columnView);
    append(_scrolledWindow);

    _window.set_child(_particleEntry);

    // Show the particle window when the user selects a particle
    _columnView.signal_activate().connect([&](guint p) {
        if (auto selected = std::dynamic_pointer_cast<NBody::Simulation::Particle>(
                _selectionModel->get_object(p))) {
            _particleEntry.bind(selected);
            _window.set_title("Particle #" + std::to_string((ENTT_ID_TYPE) selected->entity()));
            _window.show();
            _window.present();
        }
    });

    _selectionModel->signal_selection_changed().connect([&](guint p, guint n) {
        spdlog::debug("Selection changed, p={} n={}", p, n);
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

    auto keyController = Gtk::EventControllerKey::create();
    keyController->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    keyController->signal_key_released().connect([&](guint keyval, guint keycode, Gdk::ModifierType state) {

        if (keyval == GDK_KEY_Delete || keyval == GDK_KEY_BackSpace) {
            if (auto deleted = std::dynamic_pointer_cast<NBody::Simulation::Particle>(
                    _selectionModel->get_selected_item())) {
                _simulation.destroy(deleted->entity());
                _simulation.signal_particle_removed.emit(_selectionModel->get_selected());
            }
            _window.hide();
        }
    });
    add_controller(keyController);

}

