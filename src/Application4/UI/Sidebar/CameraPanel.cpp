//
// Created by jackcamp on 2/16/22.
//

#include "CameraPanel.h"

UI::CameraPanel::CameraPanel(NBody::GtkmmArcBallCamera &camera) :
        Panel("Camera"),
        _position("Position", "The location of the camera"),
        _direction("Direction", "The direction the camera faces") {

    camera.signal_positionChanged.connect(_position.widget().slot_changed);
    _position.widget().signal_changed.connect(camera.slot_moveTo);

//    camera.signal_positionChanged.connect(_position.widget().slot_update);
//    _position.widget().signal_changed.connect(camera.slot_moveTo);

    _list.append(_position);
    _list.append(_direction);
    append(_list);
}
