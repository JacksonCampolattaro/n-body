//
// Created by jackcamp on 5/23/19.
//

#ifndef N_BODY_PHYSICSSETTINGSMENU_H
#define N_BODY_PHYSICSSETTINGSMENU_H


#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include "../model/PhysicsContext.h"

class PhysicsSettingsMenu : public Gtk::Box {

public:

    PhysicsSettingsMenu();
    virtual ~PhysicsSettingsMenu();

    PhysicsContext *getPhysicsContext();

protected:

    Gtk::Box label_box = Gtk::Box(Gtk::ORIENTATION_VERTICAL);

    Gtk::Label setG_label = Gtk::Label("Gravitational Constant");
    Gtk::Label setPower_label = Gtk::Label("Power of Gravity");
    Gtk::Label setT_label = Gtk::Label("Time Step");


    Gtk::Box setting_box = Gtk::Box(Gtk::ORIENTATION_VERTICAL);

    Gtk::SpinButton G_spinButton, Power_spinButton, T_spinButton;


};


#endif //N_BODY_PHYSICSSETTINGSMENU_H
