//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_CAMERAPANEL_H
#define N_BODY_CAMERAPANEL_H

#include <gtkmm/listbox.h>

#include <NBody/View/GtkmmArcBallCamera.h>

#include "Panel.h"
#include "../Widgets/LabeledWidget.h"
#include "../Widgets/VectorEditable.h"

namespace UI {

    class CameraPanel : public Panel {
    private:

        Gtk::ListBox _list;
        LabeledWidget<PositionEditable> _position;
        LabeledWidget<DirectionEditable> _direction;

    public:

        CameraPanel(NBody::GtkmmArcBallCamera &camera);

    };

}


#endif //N_BODY_CAMERAPANEL_H
