//
// Created by jackcamp on 5/22/19.
//

#include "ViewportSettingsMenu.h"

ViewportSettingsMenu::ViewportSettingsMenu() {

    // The main box contains a vertical list of settings to change
    set_orientation(Gtk::ORIENTATION_VERTICAL);

    // This box contains width and height settings for the window
    resolutionBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);

    // Label for the width setting
    width_label.set_text("Width:");
    width_label.set_padding(10, 0);
    resolutionBox.pack_start(width_label, Gtk::PACK_SHRINK);

    // Width spinButton
    windowWidth_spinButton.set_range(100, 10000);
    windowWidth_spinButton.set_digits(0);
    windowWidth_spinButton.set_increments(10, 10);
    windowWidth_spinButton.set_value(1920);
    resolutionBox.pack_start(windowWidth_spinButton, Gtk::PACK_EXPAND_WIDGET);

    // Label for the height setting
    height_label.set_text("Height:");
    height_label.set_padding(10, 0);
    resolutionBox.pack_start(height_label, Gtk::PACK_SHRINK);

    // Height spinButton
    windowHeight_spinButton.set_range(100, 10000);
    windowHeight_spinButton.set_digits(0);
    windowHeight_spinButton.set_increments(10, 10);
    windowHeight_spinButton.set_value(1080);
    resolutionBox.pack_start(windowHeight_spinButton, Gtk::PACK_EXPAND_WIDGET);

    // Adding the box containing the spinButtons to the main box
    pack_start(resolutionBox, Gtk::PACK_SHRINK);

    // TODO There might be more settings I should add as options for the view

    // Showing all the widgets
    windowHeight_spinButton.show();
    windowWidth_spinButton.show();
    height_label.show();
    width_label.show();
    resolutionBox.show();



}

ViewportSettingsMenu::~ViewportSettingsMenu() {

}

View *ViewportSettingsMenu::getView() {

    auto dimensions = glm::ivec2(windowWidth_spinButton.get_value_as_int(), windowHeight_spinButton.get_value_as_int());
    const char *title = "n_body ViewPort";
    auto view = new View();
    view->setDimensions(dimensions)->setTitle(title);
    return view;
}
