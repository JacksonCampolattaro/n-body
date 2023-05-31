//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_PHYSICSPANEL_H
#define N_BODY_PHYSICSPANEL_H

#include <NBody/Simulation/Simulation.h>
#include "NBody/Physics/Rules/Gravity.h"

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/Entry/FloatEntry.h"

namespace UI {

    using NBody::Physics::Gravity;

    class PhysicsPanel : public BuilderWidget<Gtk::Box> {
    private:

        Gravity &_rule;

        FloatEntry &_gravityEntry;

    public:

        PhysicsPanel(Gtk::Box::BaseObjectType *cobject,
                     const Glib::RefPtr<Gtk::Builder> &builder,
                     Gravity &rule);

    };

}


#endif //N_BODY_PHYSICSPANEL_H
