//
// Created by Jackson Campolattaro on 8/2/22.
//

#ifndef N_BODY_TAGENTRY_H
#define N_BODY_TAGENTRY_H

#include "Application/UI/Widgets/Bindable.h"

#include "NBody/Physics/Mass.h"

#include <gtkmm/checkbutton.h>
#include <gtkmm/builder.h>

namespace UI {

    using NBody::Physics::ActiveTag;
    using NBody::Physics::PassiveTag;

    template<typename Tag>
    class TagEntry : public Bindable<Tag>, public Gtk::CheckButton {
    public:

        TagEntry(Gtk::CheckButton::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : Gtk::CheckButton(cobject) {

            signal_toggled().connect([&]() {
                if (get_active())
                    this->template changed<Tag>({});
                else
                    this->template changed<Tag>();
            });

        }

        void update(const Tag &value) override {
            set_active(true);
        }

        void update() override { set_active(false); };
    };

    typedef TagEntry<ActiveTag> ActiveTagEntry;
    typedef TagEntry<PassiveTag> PassiveTagEntry;
}

#endif //N_BODY_TAGENTRY_H
