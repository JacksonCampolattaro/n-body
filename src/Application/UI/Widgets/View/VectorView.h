//
// Created by jackcamp on 2/21/22.
//

#ifndef N_BODY_VECTORVIEW_H
#define N_BODY_VECTORVIEW_H

#include "Application/UI/Widgets/View/FloatView.h"

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/builder.h>

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

        FloatView<6> _x, _y, _z;

        Gtk::Label
                _open{open.value},
                _sep1{", "},
                _sep2{", "},
                _close{close.value};

        void setup() {
            add_css_class("monospace");

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

    public:

        VectorView() : Gtk::Box(Gtk::Orientation::HORIZONTAL) {
            setup();
        }

        VectorView(Gtk::Box::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
        : Gtk::Box(cobject) {
            setup();
        }

        void setValue(float x, float y, float z) {
            _x.setValue(x);
            _y.setValue(y);
            _z.setValue(z);
        }

        void setValue(const glm::vec3 &value) {
            setValue(value.x, value.y, value.z);
        };

    };

    typedef VectorView<"(", ")"> PositionView;
    typedef VectorView<"⟨", "⟩"> VelocityView;
    typedef VectorView<"[", "]"> AccelerationView;
}

#endif //N_BODY_VECTORVIEW_H
