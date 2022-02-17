#ifndef N_BODY_PANEL_H
#define N_BODY_PANEL_H

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>

namespace UI {

    class Panel : public Gtk::Box {
    private:

        Gtk::Label _title;

        Gtk::Separator _separator;

    public:

        Panel(const std::string &title) : Gtk::Box(Gtk::Orientation::VERTICAL), _title(title) {

            Pango::AttrList attributes {};
            auto fontWeight = Pango::AttrFontDesc::create_attr_weight(Pango::Weight::NORMAL);
            auto fontSize = Pango::AttrFontDesc::create_attr_scale(1.25);
            attributes.insert(fontWeight);
            attributes.insert(fontSize);

            _title.set_attributes(attributes);
            _title.set_margin(10);
            _title.set_justify(Gtk::Justification::LEFT);
            _title.set_halign(Gtk::Align::START);
            _title.set_hexpand();
            append(_title);
            append(_separator);

        }
    };

}

#endif //N_BODY_PANEL_H
