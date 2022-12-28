//
// Created by Jackson Campolattaro on 12/28/22.
//

#include "DropDownView.h"

UI::DropDownView::DropDownView(Gtk::DropDown::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
        Gtk::DropDown(cobject) {
    property_selected().signal_changed().connect([&](){
        std::dynamic_pointer_cast<Gtk::SingleSelection>(get_model())->set_selected(get_selected());
    });
}

void UI::DropDownView::set_model(const Glib::RefPtr<Gtk::SingleSelection> &model) {
    Gtk::DropDown::set_model(model);
    set_selected(model->get_selected());
}
