//
// Created by Jackson Campolattaro on 12/28/22.
//

#include "DropDownView.h"

#include <spdlog/spdlog.h>

UI::DropDownView::DropDownView(Gtk::DropDown::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
        Gtk::DropDown(cobject) {
    property_selected().signal_changed().connect([&](){
        std::dynamic_pointer_cast<Gtk::SingleSelection>(get_model())->set_selected(get_selected());
    });
}

void UI::DropDownView::set_model(const Glib::RefPtr<Gtk::SingleSelection> &model) {
    // set_model re-invokes the SingleSelection constructor (because it uses a SingleSelection internally)
    // The best way to deal with this seems to be to save the selection & restore afterward
    guint i = model->get_selected();
    Gtk::DropDown::set_model(model);
    model->set_selected(i);
    set_selected(i);
    model->signal_selection_changed().connect([&](guint, guint) {
        set_selected(model->get_selected());
    });
}
