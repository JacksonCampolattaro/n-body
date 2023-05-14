//
// Created by jackcamp on 2/16/22.
//

#include "PhysicsPanel.h"

UI::PhysicsPanel::PhysicsPanel(Gtk::Box::BaseObjectType *cobject,
                               const Glib::RefPtr<Gtk::Builder> &builder,
                               NBody::Physics::Gravity &rule) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/physics_panel.xml"),
        _rule(rule),
        _gravityEntry(getWidget<FloatEntry>("gravity-entry")) {

    _gravityEntry.set_value(rule.g());
    _gravityEntry.signal_changed.connect([&](float g) {
        rule.g() = g;
    });
}
