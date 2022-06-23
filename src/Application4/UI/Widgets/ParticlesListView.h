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
#include <gtkmm/scrolledwindow.h>
#include <giomm/listmodel.h>
#include <giomm/liststore.h>

#include <glibmm/property.h>
#include <glibmm/interface.h>
#include <glibmm/ustring.h>

#include <NBody/Simulation/Simulation.h>

#include "Application4/UI/Sidebar/Panel.h"
#include "LabeledWidget.h"

#include "ParticlesListModel.h"

namespace UI {

    class ParticlesListView : public Gtk::Box {
    private:

        NBody::Simulation &_simulation;
        Glib::RefPtr<ParticlesListModel> _particlesModel;

        Glib::RefPtr<Gtk::SignalListItemFactory> _factory = Gtk::SignalListItemFactory::create();

        Gtk::ScrolledWindow _scrolledWindow;
        Gtk::ListView _listView;

    public:

        explicit ParticlesListView(NBody::Simulation &simulation);

    private:

        void on_setup(const Glib::RefPtr<Gtk::ListItem> &listItem);

        void on_bind(const Glib::RefPtr<Gtk::ListItem> &listItem);
    };

}

#endif //N_BODY_PARTICLESLISTVIEW_H
