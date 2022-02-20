//
// Created by jackcamp on 2/19/22.
//

#include <glibmm/init.h>
#include <gtkmm/stringobject.h>
#include <spdlog/spdlog.h>
#include "Application4/UI/Sidebar/ParticlesPanel.h"
#include "ParticlesListView.h"
#include "PositionEntry.h"

UI::ParticlesListView::ParticlesListView(NBody::Simulation &simulation) :
        Gtk::Box(Gtk::Orientation::VERTICAL),
        _simulation(simulation),
        _model(Gtk::StringList::create({})),
        _factory(Gtk::SignalListItemFactory::create()),
        _listView(Gtk::NoSelection::create(_model), _factory) {


//    Glib::init();
//    Person p;
//    p.get_property<Glib::ustring>("firstname");
//    auto _p = std::make_shared<Person>();
//    _p->get_property<Glib::ustring>("firstname");
//    _particlesModel->append(_p);
//    Glib::RefPtr<Glib::ObjectBase> _p2 = _p;


    //Glib::RefPtr<Gio::ListStore<Person>> _particlesModel = Gio::ListStore<Person>::create();
    _simulation.each([&](auto entity) {
        auto particle = NBody::Simulation::Particle{_simulation, entity};
        int id = static_cast<int>(entity);
        _model->append(std::to_string(id));
        _table.insert({std::to_string(id), particle});
    });

    _factory->signal_setup().connect(sigc::mem_fun(*this, &ParticlesListView::on_setup));
    _factory->signal_bind().connect(sigc::mem_fun(*this, &ParticlesListView::on_bind));

    Gtk::Box::append(_listView);
}

void UI::ParticlesListView::on_setup(const Glib::RefPtr<Gtk::ListItem> &listItem) {
    auto label = Gtk::make_managed<Gtk::Label>("placeholder");
    auto position = Gtk::make_managed<PositionEntry>();
    position->set_hexpand(false);
    position->set_sensitive(false);
    listItem->set_child(*position);
}

void UI::ParticlesListView::on_bind(const Glib::RefPtr<Gtk::ListItem> &listItem) {

    auto positionEntry = dynamic_cast<PositionEntry *>(listItem->get_child());
    auto string = listItem->get_item()->get_property<Glib::ustring>("string");
    auto particle = _table.at(string);
    auto position = particle.get<NBody::Physics::Position>();

    positionEntry->on_update(position.x, position.y, position.z);
    //auto item = std::dynamic_pointer_cast<MyCellRenderer>(listItem->get_item());
    //item->get_property<int>("myint");
    //auto text = listItem->get_item()->get_property<Glib::ustring>("firstname");
    //listItem->get_item()->get_property<int>("myint");
    //label->set_text(listItem->get_item()->get_property<Glib::ustring>("name"));
}
