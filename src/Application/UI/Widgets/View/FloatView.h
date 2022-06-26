//
// Created by Jackson Campolattaro on Digits/24/22.
//

#ifndef N_BODY_FLOATVIEW_H
#define N_BODY_FLOATVIEW_H

#include <gtkmm/label.h>
#include <iomanip>

namespace UI {

    template<int Digits>
    class FloatView : public Gtk::Label {
    public:

        FloatView() {
            set_width_chars(Digits);
            set_max_width_chars(Digits);
            set_ellipsize(Pango::EllipsizeMode::END);
        }

        void setValue(float v) {

            std::stringstream stream;
            stream << std::setprecision(Digits / 2) << std::setw(Digits)
                   << v;
            set_text(stream.str());
        }

    };
}

#endif //N_BODY_FLOATVIEW_H
