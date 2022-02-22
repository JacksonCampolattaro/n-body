//
// Created by jackcamp on 2/16/22.
//

#include "PhysicsPanel.h"

UI::PhysicsPanel::PhysicsPanel() : Panel("Physics") {

    auto builder = Gtk::Builder::create_from_resource("/ui/physics_panel.xml");
    auto root = builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);
}
