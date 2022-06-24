//
// Created by jackcamp on 2/19/22.
//

#include <gtkmm/stringobject.h>
#include <spdlog/spdlog.h>
#include "Application/UI/Widgets/Entry/ParticleEntry.h"

#include "ParticlesListView.h"

UI::ParticlesListView::ParticlesListView(NBody::Simulation &simulation) :
        Gtk::Box(Gtk::Orientation::VERTICAL),
        _simulation(simulation),
        _particlesModel(ParticlesListModel::create(simulation)),
        _factory(Gtk::SignalListItemFactory::create()),
        _listView(Gtk::NoSelection::create(_particlesModel), _factory)
        {

    add_css_class("particles-listview");
    set_overflow(Gtk::Overflow::HIDDEN);
    _scrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);

//    _listView.set_factory(_factory);
//    auto simulationObject = Glib::make_refptr_for_instance(&simulation);
//    _listView.set_model(Gtk::NoSelection::create(simulationObject));

//    _simulation.each([&](auto entity) {
//        auto particle = new NBody::Simulation::Particle{_simulation, entity};
//        _particlesModel->append(Glib::make_refptr_for_instance(particle));
//    });

    _factory->signal_setup().connect(sigc::mem_fun(*this, &ParticlesListView::on_setup));
    _factory->signal_bind().connect(sigc::mem_fun(*this, &ParticlesListView::on_bind));

    _scrolledWindow.set_child(_listView);

    _scrolledWindow.set_vexpand();
    append(_scrolledWindow);
}

void UI::ParticlesListView::on_setup(const Glib::RefPtr<Gtk::ListItem> &listItem) {
    auto entry = Gtk::make_managed<ParticleEntry>();
    listItem->set_child(*entry);
}

void UI::ParticlesListView::on_bind(const Glib::RefPtr<Gtk::ListItem> &listItem) {

    if (auto entry = dynamic_cast<ParticleEntry *>(listItem->get_child())) {
        if (auto particle = std::dynamic_pointer_cast<NBody::Simulation::Particle>(listItem->get_item())) {
            entry->bind(*particle);
            spdlog::trace("Binding particle with widget");
        }
    }
}
