//
// Created by Jackson Campolattaro on 6/23/22.
//

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/eventcontrollerkey.h>

#include "ParticlesColumnView.h"


UI::ParticlesColumnView::ParticlesColumnView(NBody::Simulation &simulation) :
        Gtk::Box(Gtk::Orientation::VERTICAL),
        _simulation(simulation) {

    set_size_request(-1, 400);

    _particlesModel = ParticlesListModel::create(simulation);
    _selectionModel = Gtk::SingleSelection::create(
            Gtk::SortListModel::create(_particlesModel, _columnView.get_sorter()));
    _columnView.set_model(_selectionModel);

    auto idColumn = Gtk::ColumnViewColumn::create(
            "ID",
            BindableListItemFactory<ParticleIDView>::create()
    );
    auto idSorter =
            Gtk::NumericSorter<ENTT_ID_TYPE>::create(
                    Gtk::ClosureExpression<ENTT_ID_TYPE>::create([](const Glib::RefPtr<Glib::ObjectBase> &item) {
                        auto particle = std::dynamic_pointer_cast<NBody::Simulation::Particle>(item);
                        assert(particle);
                        return (ENTT_ID_TYPE) particle->entity();
                    })
            );
    idColumn->set_sorter(idSorter);
    _columnView.append_column(idColumn);
    _columnView.append_column(Gtk::ColumnViewColumn::create(
            "Appearance",
            BindableListItemFactory<ParticleIconView>::create()
    ));
    _columnView.sort_by_column(idColumn, Gtk::SortType::ASCENDING);
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

    _columnView.signal_activate().connect([&](guint p) {
        if (auto selected = std::dynamic_pointer_cast<NBody::Simulation::Particle>(
                _selectionModel->get_object(p))) {
            signal_openParticle.emit(selected);
        }
    });

    auto keyController = Gtk::EventControllerKey::create();
    keyController->signal_key_released().connect([&](guint keyval, guint keycode, Gdk::ModifierType state) {

        if (keyval == GDK_KEY_Delete || keyval == GDK_KEY_BackSpace) {
            if (auto deleted =
                    std::dynamic_pointer_cast<NBody::Simulation::Particle>(_selectionModel->get_selected_item())) {
                _simulation.removeParticle(deleted->entity());
            }
        }
        _selectionModel->set_selected(GTK_INVALID_LIST_POSITION);
    });
    add_controller(keyController);

}

