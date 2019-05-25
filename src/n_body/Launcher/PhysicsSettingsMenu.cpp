//
// Created by jackcamp on 5/23/19.
//

#include "PhysicsSettingsMenu.h"

PhysicsSettingsMenu::PhysicsSettingsMenu() {

    // Left side contains labels, right side contains controls
    set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    pack_start(label_box);
    pack_start(setting_box);

    // Adding the labels
    label_box.pack_start(setG_label);
    label_box.pack_start(setPower_label);
    label_box.pack_start(setT_label);

    // Adding the settings
    G_spinButton.set_range(-10, 10);
    G_spinButton.set_digits(3);
    G_spinButton.set_increments(0.01, 0.1);
    G_spinButton.set_value(1);
    setting_box.pack_start(G_spinButton);

    Power_spinButton.set_range(-10, 10);
    Power_spinButton.set_digits(2);
    Power_spinButton.set_increments(0.1, 1);
    Power_spinButton.set_value(2);
    setting_box.pack_start(Power_spinButton);

    T_spinButton.set_range(0, 100);
    T_spinButton.set_digits(3);
    T_spinButton.set_increments(0.001, 0.01);
    T_spinButton.set_value(0.01);
    setting_box.pack_start(T_spinButton);

    // Making sure everything is drawn
    show_all_children();
}

PhysicsSettingsMenu::~PhysicsSettingsMenu() {

}

PhysicsContext *PhysicsSettingsMenu::getPhysicsContext() {

    PhysicsContext *physicsContext;
    physicsContext->setG(G_spinButton.get_value())->setPower(Power_spinButton.get_value())->setT(T_spinButton.get_value());
    return physicsContext;
}
