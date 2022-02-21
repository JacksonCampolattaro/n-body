//
// Created by jackcamp on 2/19/22.
//

#include <glibmm/init.h>
#include <gtkmm/stringobject.h>
#include <spdlog/spdlog.h>
#include "Application4/UI/Sidebar/ParticlesPanel.h"
#include "ParticlesListView.h"
#include "PositionEntry.h"
#include "ParticleEntry.h"

UI::ParticlesListView::ParticlesListView(NBody::Simulation &simulation) :
        Gtk::Box(Gtk::Orientation::VERTICAL),
        _simulation(simulation),
        _model(Gtk::StringList::create({})),
        _factory(Gtk::SignalListItemFactory::create()),
        _listView(Gtk::NoSelection::create(_particlesModel), _factory) {


    _simulation.each([&](auto entity) {
        auto particle = NBody::Simulation::Particle{_simulation, entity};
        _particlesModel->append(Glib::make_refptr_for_instance(new GtkParticleHandle(particle)));
    });

    _factory->signal_setup().connect(sigc::mem_fun(*this, &ParticlesListView::on_setup));
    _factory->signal_bind().connect(sigc::mem_fun(*this, &ParticlesListView::on_bind));

    Gtk::Box::append(_listView);
}

void UI::ParticlesListView::on_setup(const Glib::RefPtr<Gtk::ListItem> &listItem) {
    auto entry = Gtk::make_managed<ParticleEntry>();
    listItem->set_child(*entry);
}

void UI::ParticlesListView::on_bind(const Glib::RefPtr<Gtk::ListItem> &listItem) {

    if (auto entry = dynamic_cast<ParticleEntry *>(listItem->get_child())) {
        if (auto particleObject = std::dynamic_pointer_cast<GtkParticleHandle>(listItem->get_item())) {
            auto particle = particleObject->get_property<NBody::Simulation::Particle>("particle");
            entry->bind(particle);
            spdlog::debug("binding");
        }
    }
}

UI::GtkParticleHandle::GtkParticleHandle(NBody::Simulation::Particle particle) :
        Glib::ObjectBase(typeid(GtkParticleHandle)),
        Glib::Object(),
        _particle(*this, "particle", particle),
        _name(*this, "name", "placeholder") {}
