//
// Created by Jackson Campolattaro on 6/23/22.
//

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>
#include <gtkmm/signallistitemfactory.h>
#include <gtkmm/singleselection.h>

#include "ParticlesColumnView.h"


UI::ParticlesColumnView::ParticlesColumnView(NBody::Simulation &simulation) :
        Gtk::Box(Gtk::Orientation::VERTICAL),
        _simulation(simulation),
        _particlesModel(ParticlesListModel::create(simulation)) {

    set_size_request(-1, 400);

    _columnView.set_model(Gtk::SingleSelection::create(_particlesModel));

    _columnView.append_column(Gtk::ColumnViewColumn::create("Active Mass", BindableListItemFactory<ParticleActiveMassBindable>::create()));
    _columnView.append_column(Gtk::ColumnViewColumn::create("Passive Mass", BindableListItemFactory<ParticlePassiveMassBindable>::create()));
    _columnView.append_column(Gtk::ColumnViewColumn::create("Position", BindableListItemFactory<ParticlePositionBindable>::create()));
    _columnView.append_column(Gtk::ColumnViewColumn::create("Velocity", BindableListItemFactory<ParticleVelocityBindable>::create()));

    _scrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    _scrolledWindow.set_vexpand();
    _scrolledWindow.set_child(_columnView);
    append(_scrolledWindow);
}

