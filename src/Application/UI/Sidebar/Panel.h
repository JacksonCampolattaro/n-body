#ifndef N_BODY_PANEL_H
#define N_BODY_PANEL_H

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/builder.h>

namespace UI {

    class Panel : public Gtk::Box {
    private:

        Gtk::Label _title;

        Gtk::Separator _separator;

        Glib::RefPtr<Gtk::Builder> _builder;

    protected:

        Gtk::Box &_contents;

    public:

        Panel(const std::string &title) : Gtk::Box(Gtk::Orientation::VERTICAL),
        _builder(Gtk::Builder::create_from_resource("/ui/panel.xml")),
        _contents(*_builder->get_widget<Gtk::Box>("box")),
        _title(title) {

            auto root = _builder->get_widget<Gtk::Widget>("root");
            append(*root);

            auto label = _builder->get_widget<Gtk::Label>("label");
            label->set_text(title);

        }
    };

}

#endif //N_BODY_PANEL_H
