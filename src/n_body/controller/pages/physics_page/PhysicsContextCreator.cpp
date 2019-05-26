//
// Created by jackcamp on 5/24/19.
//

#include "PhysicsContextCreator.h"

PhysicsContextCreator::PhysicsContextCreator() {

    // Setting up the main box

    set_orientation(Gtk::ORIENTATION_VERTICAL);
    set_margin_top(20);
    set_margin_bottom(20);
    set_margin_left(30);
    set_margin_right(30);
    set_spacing(10);


    // Adding the header

    header_label.set_use_markup(true);
    header_label.set_alignment(Gtk::ALIGN_START);


    // Adding the description

    description_label.set_use_markup(true);
    description_label.set_alignment(Gtk::ALIGN_START);
    description_label.set_margin_bottom(10);


    // Configuring Boxes

    G_box.set_can_focus(false);
    G_box.set_homogeneous(true);

    P_box.set_can_focus(false);
    P_box.set_homogeneous(true);

    T_box.set_can_focus(false);
    T_box.set_homogeneous(true);


    // Adding labels

    G_label.set_can_focus(false);
    G_label.set_halign(Gtk::ALIGN_START);
    G_label.set_hexpand(true);
    G_label.set_xalign(0);
    G_label.set_can_focus(false);
    G_box.pack_start(G_label);

    P_label.set_can_focus(false);
    P_label.set_halign(Gtk::ALIGN_START);
    P_label.set_hexpand(true);
    P_label.set_xalign(0);
    P_label.set_can_focus(false);
    P_box.pack_start(P_label);

    T_label.set_can_focus(false);
    T_label.set_halign(Gtk::ALIGN_START);
    T_label.set_hexpand(true);
    T_label.set_xalign(0);
    T_label.set_can_focus(false);
    T_box.pack_start(T_label);


    // Adding spinButtons

    G_spinButton.set_range(-10, 10);
    G_spinButton.set_digits(3);
    G_spinButton.set_increments(0.01, 0.1);
    G_spinButton.set_value(1);
    G_box.pack_start(G_spinButton);

    P_spinButton.set_range(-10, 10);
    P_spinButton.set_digits(1);
    P_spinButton.set_increments(0.1, 1);
    P_spinButton.set_value(2);
    P_box.pack_start(P_spinButton, Gtk::PACK_EXPAND_WIDGET);

    T_spinButton.set_range(0, 100);
    T_spinButton.set_digits(3);
    T_spinButton.set_increments(0.001, 0.01);
    T_spinButton.set_value(0.01);
    T_box.pack_start(T_spinButton, Gtk::PACK_EXPAND_WIDGET);


    // Adding everything to the main box

    pack_start(header_label, Gtk::PACK_SHRINK);
    pack_start(description_label, Gtk::PACK_SHRINK);
    pack_start(G_box, Gtk::PACK_SHRINK);
    pack_start(P_box, Gtk::PACK_SHRINK);
    pack_start(T_box, Gtk::PACK_SHRINK);


    // Showing everything

    show_all_children();

}

PhysicsContext *PhysicsContextCreator::createPhysicsContext() {

    auto physicsContext = new PhysicsContext;
    physicsContext->setG(G_spinButton.get_value())->setPower(P_spinButton.get_value())->setT(T_spinButton.get_value());
    return physicsContext;
}
