//
// Created by jackcamp on 5/22/19.
//

#ifndef N_BODY_VIEWSETTINGSMENU_H
#define N_BODY_VIEWSETTINGSMENU_H

#include <gtkmm/box.h>
#include <gtkmm/spinbutton.h>
#include "../view/View.h"

class ViewSettingsMenu : public Gtk::Box {

public:

    ViewSettingsMenu();
    virtual ~ViewSettingsMenu();

    View *getView();

protected:

    // Used to select the resolution of the window
    Gtk::Box resolutionBox;
    Gtk::Label width_label, height_label;

    Gtk::SpinButton windowHeight_spinButton, windowWidth_spinButton;
};


#endif //N_BODY_VIEWSETTINGSMENU_H
