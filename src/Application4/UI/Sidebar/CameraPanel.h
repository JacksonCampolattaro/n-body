//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_CAMERAPANEL_H
#define N_BODY_CAMERAPANEL_H

#include <gtkmm/listbox.h>
#include <gtkmm/colorbutton.h>

#include <NBody/View/GtkmmArcBallCamera.h>

#include "Panel.h"
#include "../Widgets/LabeledWidget.h"
#include "Application4/UI/Widgets/Entry/VectorEntry.h"

namespace UI {

    class CameraPanel : public Panel {

    public:

        CameraPanel(NBody::GtkmmArcBallCamera &camera);

    };

}


#endif //N_BODY_CAMERAPANEL_H
