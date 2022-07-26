//
// Created by Jackson Campolattaro on Digits/24/22.
//

#ifndef N_BODY_FLOATVIEW_H
#define N_BODY_FLOATVIEW_H

#include <gtkmm/label.h>
#include <gtkmm/builder.h>

#include <iomanip>

namespace UI {

    template<int Digits>
    class FloatView : public Gtk::Label {
    private:

        void setup() {
            set_width_chars(Digits);
            set_max_width_chars(Digits);
            set_ellipsize(Pango::EllipsizeMode::END);
            set_xalign(1.0f);
        }

    public:

        FloatView() { setup(); }

        FloatView(Gtk::Label::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : Gtk::Label(cobject) { setup(); }

        void setValue(float v) {

            std::stringstream stream;
            stream << std::setprecision(Digits / 2) << std::setw(Digits) << std::fixed
                   << v;
            set_text(stream.str());
        }

    };

    typedef FloatView<4> CoarseFloatView;
    typedef FloatView<10> PreciseFloatView;
}

#endif //N_BODY_FLOATVIEW_H
