//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_PHYSICSPANEL_H
#define N_BODY_PHYSICSPANEL_H

#include <NBody/Simulation/Simulation.h>
#include <NBody/Physics/Rule.h>

#include "Panel.h"

namespace UI {

    using NBody::Physics::Rule;

    class PhysicsPanel : public Panel {
    private:

        Rule &_rule;

    public:

        PhysicsPanel(Rule &rule);

    };

}


#endif //N_BODY_PHYSICSPANEL_H
