//
// Created by jackcamp on 5/24/19.
//

#include "ViewCreator.h"

ViewCreator::ViewCreator() {

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


    // Configuring boxes

    windowSize_box.set_spacing(5);


    // Configuring labels

    windowSize_label.set_use_markup(true);
    windowSize_label.set_margin_right(10);
    windowWidth_label.set_use_markup(true);
    windowHeight_label.set_use_markup(true);


    // Configuring spinbuttons

    windowWidth_spinButton.set_range(100, 10000);
    windowWidth_spinButton.set_digits(0);
    windowWidth_spinButton.set_increments(10, 10);
    windowWidth_spinButton.set_value(1920);

    windowHeight_spinButton.set_range(100, 10000);
    windowHeight_spinButton.set_digits(0);
    windowHeight_spinButton.set_increments(10, 10);
    windowHeight_spinButton.set_value(1080);


    // Filling the window size box

    windowSize_box.pack_start(windowSize_label, Gtk::PACK_SHRINK);
    windowSize_box.pack_start(windowWidth_label, Gtk::PACK_SHRINK);
    windowSize_box.pack_start(windowWidth_spinButton, Gtk::PACK_EXPAND_WIDGET);
    windowSize_box.pack_start(windowHeight_label, Gtk::PACK_SHRINK);
    windowSize_box.pack_start(windowHeight_spinButton, Gtk::PACK_EXPAND_WIDGET);


    // Filling the main box

    pack_start(header_label, Gtk::PACK_SHRINK);
    pack_start(description_label, Gtk::PACK_SHRINK);
    pack_start(windowSize_box, Gtk::PACK_SHRINK);


    // Making sure everything is drawn

    show_all_children();

}

View *ViewCreator::createView() {

    auto dimensions = glm::ivec2(windowWidth_spinButton.get_value_as_int(), windowHeight_spinButton.get_value_as_int());
    const char *title = "n_body ViewPort";
    auto view = new View(dimensions.x, dimensions.y, title);
    return view;
}
