//
// Created by jackcamp on 5/24/19.
//

#ifndef N_BODY_VIEWCREATOR_H
#define N_BODY_VIEWCREATOR_H


#include <gtkmm/box.h>
#include <gtkmm/switch.h>
#include <gtkmm/label.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/spinbutton.h>
#include "../../../view/View.h"

class ViewCreator : public Gtk::Box {

public:

    ViewCreator();

    View *createView();

protected:

    Gtk::Label
            header_label = Gtk::Label("<b>View Properties</b>"),
            description_label = Gtk::Label("<i>Fields here affect the video output of the simulation.</i>");

    Gtk::Box
            windowSize_box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);

    Gtk::Label
            windowSize_label = Gtk::Label("Dimensions"),
            windowWidth_label = Gtk::Label("<i> W </i>"),
            windowHeight_label = Gtk::Label("<i> H </i>");

    Gtk::SpinButton
            windowWidth_spinButton,
            windowHeight_spinButton;
};


#endif //N_BODY_VIEWCREATOR_H
