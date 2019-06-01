//
// Created by jackcamp on 5/24/19.
//

#ifndef N_BODY_PHYSICSCONTEXTCREATOR_H
#define N_BODY_PHYSICSCONTEXTCREATOR_H


#include <gtkmm/box.h>
#include <gtkmm/listbox.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/frame.h>
#include <gtkmm/scrolledwindow.h>
#include "../../../model/PhysicsContext.h"

class PhysicsContextCreator : public Gtk::Box {

public:

    PhysicsContextCreator();

    PhysicsContext *createPhysicsContext();


private:

    Gtk::Label
            header_label = Gtk::Label("<b>Physics Properties</b>"),
            description_label = Gtk::Label("<i>Set values here to affect how the world's physicsContext will act.</i>");

    Gtk::Box
            G_box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL),
            P_box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL),
            T_box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);
    Gtk::Label
            G_label = Gtk::Label("Constant of Gravity"),
            P_label = Gtk::Label("Power of gravity"),
            T_label = Gtk::Label("Time step");
    Gtk::SpinButton
            G_spinButton,
            P_spinButton,
            T_spinButton;


};


#endif //N_BODY_PHYSICSCONTEXTCREATOR_H
