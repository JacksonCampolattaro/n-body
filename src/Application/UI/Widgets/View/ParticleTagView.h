//
// Created by Jackson Campolattaro on 8/2/22.
//

#ifndef N_BODY_PARTICLETAGVIEW_H
#define N_BODY_PARTICLETAGVIEW_H

#include <gtkmm/label.h>
#include <gtkmm/builder.h>

#include <NBody/Physics/Mass.h>

#include "Application/UI/Widgets/Bindable.h"

namespace UI {

    using NBody::Physics::ActiveTag;
    using NBody::Physics::PassiveTag;

    template<typename Tag>
    class ParticleTagView : public Bindable<Tag>, public Gtk::Label {
    public:

        ParticleTagView(Gtk::Label::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : Gtk::Label(cobject) {
            set_text("✓");
        }

        ParticleTagView() : Gtk::Label("✓") {}

        void update(const Tag &value) override { show(); }

        void update() override { hide(); };

    };

    typedef ParticleTagView<ActiveTag> ParticleActiveTagView;
    typedef ParticleTagView<PassiveTag> ParticlePassiveTagView;

}

#endif //N_BODY_PARTICLETAGVIEW_H
