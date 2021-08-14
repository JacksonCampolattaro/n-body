//
// Created by jackcamp on 8/14/21.
//

#ifndef N_BODY_LABELEDWIDGET_H
#define N_BODY_LABELEDWIDGET_H

#include <gtkmm/box.h>
#include <gtkmm/label.h>

namespace Interface {

    template<class W>
    class LabeledWidget : public Gtk::Box {
    private:

        Gtk::Label _label;
        W _widget;

    public:

        template<typename...T>
        LabeledWidget(const Glib::ustring &text, T... args) : Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL),
                                                              _label(text), _widget(args...) {

            set_spacing(10);
            set_margin_left(5);
            set_margin_right(5);
            set_margin_top(5);
            set_margin_bottom(5);

            add(_label);
            _label.set_justify(Gtk::JUSTIFY_LEFT);
            _label.set_xalign(0);
            _label.set_hexpand(false);
            _label.show();

            add(_widget);
            _widget.set_halign(Gtk::Align::ALIGN_END);
            _widget.set_hexpand(true);
            _widget.show();
        };

        W &widget() { return _widget; }

    };
}


#endif //N_BODY_LABELEDWIDGET_H
