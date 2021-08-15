//
// Created by jackcamp on 8/12/21.
//

#ifndef N_BODY_INTERACTIVE_H
#define N_BODY_INTERACTIVE_H

#include <NBody/Simulation/Simulation.h>
#include <NBody/View/View.h>

#include <Magnum/Math/Matrix4.h>

#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>
#include <gtkmm/paned.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/popovermenu.h>
#include <gtkmm/builder.h>

#include "Interface.h"
#include "ConfigurationPanel/ConfigurationPanel.h"

namespace Interface {

    using NBody::Simulation;

    class Interactive : public Interface {
    private:

        Gtk::HeaderBar _headerbar;
        Gtk::MenuButton _menubutton;
        Gtk::PopoverMenu _popoverMenu;
        Gtk::Box _menuBox{Gtk::Orientation::ORIENTATION_VERTICAL};
        Gtk::Button _example{"example text"};
        Gtk::Button _example2{"more example text"};

        Gtk::Paned _paned;

        NBody::View _view;
        ConfigurationPanel _configurationPanel;

    public:

        Interactive(Simulation &simulation);

    public:
        // Signals

        sigc::signal<void(Magnum::Matrix4)> signal_moveCamera;

    public:
        // Slots

        bool on_keyPressEvent(GdkEventKey *event);

    };
}


#endif //N_BODY_INTERACTIVE_H
