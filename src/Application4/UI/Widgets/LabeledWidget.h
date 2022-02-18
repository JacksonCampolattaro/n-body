//
// Created by jackcamp on 8/14/21.
//

#ifndef N_BODY_LABELEDWIDGET_H
#define N_BODY_LABELEDWIDGET_H

#include <gtkmm/box.h>
#include <gtkmm/label.h>

namespace UI {

    template<class W>
    class LabeledWidget : public Gtk::Box {
    private:

        Gtk::Box _textBox;
        Gtk::Label _name, _description;
        W _widget;

    public:

        template<typename...T>
        LabeledWidget(const Glib::ustring &name, const Glib::ustring &description, T... args) :
                Gtk::Box(Gtk::Orientation::HORIZONTAL),
                _textBox(Gtk::Orientation::VERTICAL),
                _name(name), _description(description),
                _widget(args...) {

            _name.set_halign(Gtk::Align::START);
            _name.set_justify(Gtk::Justification::LEFT);
            _textBox.append(_name);

            _description.get_style_context()->add_class("description");
            _description.set_halign(Gtk::Align::START);
            _description.set_justify(Gtk::Justification::LEFT);
            _textBox.append(_description);

            _textBox.set_halign(Gtk::Align::START);
            append(_textBox);

            _widget.set_valign(Gtk::Align::CENTER);
            _widget.set_vexpand(false);
            _widget.set_hexpand();
            _widget.set_halign(Gtk::Align::END);
            append(_widget);

            set_spacing(15);
        };

        W &widget() { return _widget; }

    };
}


#endif //N_BODY_LABELEDWIDGET_H
