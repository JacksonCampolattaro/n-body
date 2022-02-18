//
// Created by jackcamp on 2/16/22.
//

#include "CameraPanel.h"

UI::CameraPanel::CameraPanel(NBody::GtkmmArcBallCamera &camera) :
        Panel("Camera"),
        _position("Position", "The location of the camera", 1.0, 2.0, 3.0) {

    _list.append(_position);
    append(_list);
}
