//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_PHYSICSPANEL_H
#define N_BODY_PHYSICSPANEL_H

#include <NBody/Simulation/Simulation.h>
#include <NBody/Physics/Rule.h>

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/Entry/FloatEntry.h"

namespace UI {

    using NBody::Physics::Rule;

    class PhysicsPanel : public BuilderWidget<Gtk::Box> {
    private:

        Rule &_rule;

        FloatEntry &_gravityEntry;

    public:

        PhysicsPanel(Gtk::Box::BaseObjectType *cobject,
                     const Glib::RefPtr<Gtk::Builder> &builder,
                     Rule &rule);

    };

}


#endif //N_BODY_PHYSICSPANEL_H
