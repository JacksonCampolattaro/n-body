//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_CAMERAPANEL_H
#define N_BODY_CAMERAPANEL_H

#include <NBody/View/GtkmmArcBallCamera.h>

#include "Panel.h"

namespace UI {

    class CameraPanel : public Panel {
    public:

        CameraPanel(NBody::GtkmmArcBallCamera &camera);

    };

}


#endif //N_BODY_CAMERAPANEL_H
