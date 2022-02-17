//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_PARTICLESPANEL_H
#define N_BODY_PARTICLESPANEL_H

#include <NBody/Simulation/Simulation.h>

#include "Panel.h"

namespace UI {

    class ParticlesPanel : public Panel {
    public:

        ParticlesPanel(NBody::Simulation &simulation);

    };

}


#endif //N_BODY_PARTICLESPANEL_H
