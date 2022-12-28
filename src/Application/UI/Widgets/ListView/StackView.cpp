//
// Created by Jackson Campolattaro on 12/26/22.
//

#include "StackView.h"

#include <cassert>

UI::StackView::StackView(Gtk::Stack::BaseObjectType *cobject,
                         const Glib::RefPtr<Gtk::Builder> &builder) :
        Gtk::Stack(cobject) {

    signal_realize().connect([&]() { update(); });

    //    // Create & add all the widgets that will be switched between using the factory
    //    for (int i = 0; i < model->get_n_items(); ++i) {
    //        auto widget = factory(model->get_object(i));
    //        _map.emplace(model->get_object(i), widget);
    //        add(*widget);
    //    }
    //
    //    // Set the visible widget to match the model
    //    set_visible_child(*_map[model->get_selected_item()]);
    //    model->signal_selection_changed().connect([&](guint, guint){
    //        set_visible_child(*_map[model->get_selected_item()]);
    //    });
}

void UI::StackView::set_model(const Glib::RefPtr<Gtk::SingleSelection> &model) {
    _model = model;
    _model->signal_items_changed().connect([&](guint, guint, guint) { update(); });
    _model->signal_selection_changed().connect([&](guint, guint) { update(); });
}

void UI::StackView::set_expression(const Glib::RefPtr<Gtk::Expression<Gtk::Widget *>> &factory) { _factory = factory; }

void UI::StackView::update() {
    if (!_model || !_factory) return;

    // Make sure every item in the model has a widget
    for (int i = 0; i < _model->get_n_items(); ++i) {

        // No need to re-create an existing widget
        if (_map.contains(_model->get_object(i))) continue;

        auto widget = *_factory->evaluate(_model->get_object(i));
        _map.emplace(_model->get_object(i), widget);
        add(*widget);
    }

    // Update which widget is showing
    set_visible_child(*_map[_model->get_selected_item()]);
}
