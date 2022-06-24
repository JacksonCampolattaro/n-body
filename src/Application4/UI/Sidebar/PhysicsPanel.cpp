//
// Created by jackcamp on 2/16/22.
//

#include "PhysicsPanel.h"
#include "Application4/UI/Widgets/Entry/FloatEntry.h"

UI::PhysicsPanel::PhysicsPanel(NBody::Physics::Rule &rule) : Panel("Physics"), _rule(rule) {

    auto builder = Gtk::Builder::create_from_resource("/ui/physics_panel.xml");
    auto root = builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);

    auto &gEntry = *Gtk::Builder::get_widget_derived<FloatEntry>(builder, "GFloatEntry");
    gEntry.set_value(rule.g());
    gEntry.signal_changed.connect([&] (float g) {
        rule.g() = g;
    });
}
