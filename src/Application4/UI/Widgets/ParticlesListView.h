//
// Created by jackcamp on 2/19/22.
//

#ifndef N_BODY_PARTICLESLISTVIEW_H
#define N_BODY_PARTICLESLISTVIEW_H

#include <gtkmm/listbox.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/switch.h>
#include <gtkmm/signallistitemfactory.h>
#include <gtkmm/listview.h>
#include <gtkmm/noselection.h>
#include <gtkmm/stringlist.h>
#include <gtkmm/cellrenderer.h>
#include <giomm/listmodel.h>
#include <giomm/liststore.h>

#include <glibmm.h>
#include <glibmm/property.h>

#include <NBody/Simulation/Simulation.h>

#include "Application4/UI/Sidebar/Panel.h"
#include "LabeledWidget.h"

namespace UI {

    /*
    class GtkParticleHandle : public Gtk::CellRenderer {
    public:
        //NBody::Simulation::Particle particle;

        GtkParticleHandle(NBody::Simulation::Particle particle) :
                Glib::ObjectBase(typeid(GtkParticleHandle)),
                Gtk::CellRenderer(),
                _name(*this, "name", "[placeholder]")
        //particle(particle)
        {
        };

        virtual ~GtkParticleHandle() {}

        Glib::Property<Glib::ustring> _name;
    };
    */

    class ParticlesListView : public Gtk::Box {
    private:

        std::map<Glib::ustring, NBody::Simulation::Particle> _table;

        NBody::Simulation &_simulation;

        Glib::RefPtr<Gtk::StringList> _model = Gtk::StringList::create({"a", "b"});
        Glib::RefPtr<Gtk::SignalListItemFactory> _factory = Gtk::SignalListItemFactory::create();

        Gtk::ListView _listView;

    public:

        explicit ParticlesListView(NBody::Simulation &simulation);

    private:

        void on_setup(const Glib::RefPtr<Gtk::ListItem> &listItem);

        void on_bind(const Glib::RefPtr<Gtk::ListItem> &listItem);
    };

}

#endif //N_BODY_PARTICLESLISTVIEW_H
