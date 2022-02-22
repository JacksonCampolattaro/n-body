//
// Created by jackcamp on 2/21/22.
//

#ifndef N_BODY_VECTORVIEW_H
#define N_BODY_VECTORVIEW_H

#include <gtkmm/box.h>
#include <gtkmm/label.h>

#include <glm/vec3.hpp>

#include <iomanip>

namespace UI {

    // A neat C++20 trick from https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
    template<size_t N>
    struct StringLiteral {
        constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }
        char value[N];
    };

    template<StringLiteral open, StringLiteral close>
    class VectorView : public Gtk::Box {
    private:

        Gtk::Label
                _open{open.value},
                _x{"[X]"},
                _sep1{", "},
                _y{"[Y]"},
                _sep2{", "},
                _z{"[Z]"},
                _close{close.value};

    public:

        VectorView() : Gtk::Box(Gtk::Orientation::HORIZONTAL) {

            add_css_class("monospace");
            add_css_class("caption");

            _x.set_width_chars(5);
            _y.set_width_chars(5);
            _z.set_width_chars(5);

            _x.set_max_width_chars(5);
            _y.set_max_width_chars(5);
            _z.set_max_width_chars(5);

            _x.set_ellipsize(Pango::EllipsizeMode::END);
            _y.set_ellipsize(Pango::EllipsizeMode::END);
            _z.set_ellipsize(Pango::EllipsizeMode::END);

            _open.add_css_class("dim-label");
            _sep1.add_css_class("dim-label");
            _sep2.add_css_class("dim-label");
            _close.add_css_class("dim-label");

            append(_open);
            append(_x);
            append(_sep1);
            append(_y);
            append(_sep2);
            append(_z);
            append(_close);

            setValue({0, 0, 0});
        }

        void setValue(float x, float y, float z) {

            std::stringstream stream;
            stream << std::setprecision(2);

            stream.str(std::string());
            stream << std::setw(5) << x;
            _x.set_label(stream.str());
            stream.clear();

            stream.str(std::string());
            stream << std::setw(5) << y;
            _y.set_label(stream.str());
            stream.clear();

            stream.str(std::string());
            stream << std::setw(5) << z;
            _z.set_label(stream.str());
            stream.clear();

        }

        void setValue(const glm::vec3 &value) {
            setValue(value.x, value.y, value.z);
        };

    };

    typedef VectorView<"(", ")"> PositionView;
    typedef VectorView<"⟨", "⟩"> VelocityView;
}

#endif //N_BODY_VECTORVIEW_H
