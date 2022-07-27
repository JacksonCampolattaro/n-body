//
// Created by Jackson Campolattaro on 7/27/22.
//

#ifndef N_BODY_TIMEVIEW_H
#define N_BODY_TIMEVIEW_H

#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>

#include <iomanip>

namespace UI {

    class TimeView : public Gtk::Box {
    private:

        Glib::RefPtr<Gtk::Builder> _builder;

        Gtk::Label &_minutesLabel;
        Gtk::Label &_secondsLabel;
        Gtk::Label &_millisecondsLabel;

    public:

        TimeView(Gtk::Box::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : Gtk::Box(cobject),
                  _builder(Gtk::Builder::create_from_resource("/ui/time_view.xml")),
                  _minutesLabel(*_builder->get_widget<Gtk::Label>("minutes-label")),
                  _secondsLabel(*_builder->get_widget<Gtk::Label>("seconds-label")),
                  _millisecondsLabel(*_builder->get_widget<Gtk::Label>("milliseconds-label")) {
            append(*_builder->get_widget<Gtk::Widget>("root"));
        }

        void setValue(std::chrono::duration<double> v) {

            auto minutes = std::chrono::duration_cast<std::chrono::minutes>(v);
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(v - minutes);
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(v - minutes - seconds);

            _minutesLabel.set_text(std::to_string(minutes.count()));
            _secondsLabel.set_text(std::to_string(seconds.count()));
            _millisecondsLabel.set_text(std::to_string(milliseconds.count()));

            if (_secondsLabel.get_text().size() == 1) _secondsLabel.set_text("0" + _secondsLabel.get_text());
        }

    };
}

#endif //N_BODY_TIMEVIEW_H
