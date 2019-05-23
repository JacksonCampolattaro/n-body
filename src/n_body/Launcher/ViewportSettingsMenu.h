//
// Created by jackcamp on 5/22/19.
//

#ifndef N_BODY_VIEWPORTSETTINGSMENU_H
#define N_BODY_VIEWPORTSETTINGSMENU_H

#include <gtkmm/box.h>
#include <gtkmm/spinbutton.h>
#include "../view/View.h"

class ViewportSettingsMenu : public Gtk::Box {

public:

    ViewportSettingsMenu();
    virtual ~ViewportSettingsMenu();

    View *getView();

protected:

    // Used to select the resolution of the window
    Gtk::Box resolutionBox;
    Gtk::Label width_label, height_label;

    Gtk::SpinButton windowHeight_spinButton, windowWidth_spinButton;
};


#endif //N_BODY_VIEWPORTSETTINGSMENU_H
