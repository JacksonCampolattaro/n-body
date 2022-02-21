//
// Created by jackcamp on 2/16/22.
//

#include "CameraPanel.h"

UI::CameraPanel::CameraPanel(NBody::GtkmmArcBallCamera &camera) :
        Panel("Camera"),
        _camera(camera),
        _position("Position", "The location of the camera"),
        _direction("Direction", "The direction the camera faces"),
        _backgroundColor("Background Color", "The color of the background"){

    _backgroundColor.widget().signal_color_set().connect(sigc::mem_fun(*this, &UI::CameraPanel::on_backgroundColorChanged));
    _backgroundColor.widget().set_rgba({0.12, 0.12, 0.1, 1.0});

    camera.signal_positionChanged.connect(_position.widget().slot_changed);
    _position.widget().signal_changed.connect(camera.slot_moveTo);

    camera.signal_directionChanged.connect(_direction.widget().slot_changed);
    _direction.widget().set_sensitive(false);

    _list.append(_position);
    _list.append(_direction);
    _list.append(_backgroundColor);
    append(_list);
}

void UI::CameraPanel::on_backgroundColorChanged() {
    auto color = _backgroundColor.widget().get_rgba();
    _camera.setBackgroundColor(color);
}
