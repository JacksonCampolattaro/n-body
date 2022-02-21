//
// Created by jackcamp on 8/17/21.
//

#include "GtkmmArcBallCamera.h"

void NBody::GtkmmArcBallCamera::moveTo(float x, float y, float z) {

    spdlog::debug("Moving the camera to ({}, {}, {})", x, y, z);

    _targetPosition.x() = x;
    _targetPosition.y() = y;
    _targetPosition.z() = z;
    _currentPosition = _targetPosition;
    updateInternalTransformations();

    signal_changed.emit();
    signal_positionChanged.emit(x, y, z);
}

bool NBody::GtkmmArcBallCamera::on_scroll(double dx, double dy) {
    spdlog::debug("Scroll signal received: dx={}, dy={}", dx, dy);

    zoom((float) -dy);
    signal_changed.emit();
    return false;
}

void NBody::GtkmmArcBallCamera::on_leftButtonPress(int _, double x, double y) {
    spdlog::debug("Left click signal received: x={}, y={}", x, y);

    initTransformation({static_cast<int>(x), static_cast<int>(y)});
    _mode = TransformationMode::ROTATE;
    signal_changed.emit();
}

void NBody::GtkmmArcBallCamera::on_middleButtonPress(int _, double x, double y) {
    spdlog::debug("Middle click signal received: x={}, y={}", x, y);

    initTransformation({static_cast<int>(x), static_cast<int>(y)});
    _mode = TransformationMode::TRANSLATE;
    signal_changed.emit();
}

void NBody::GtkmmArcBallCamera::on_buttonRelease(int _, double x, double y) {
    spdlog::debug("Mouse button released");

    _mode = TransformationMode::NONE;
    signal_changed.emit();
}

void NBody::GtkmmArcBallCamera::on_MouseMotion(double x, double y) {

    if (_mode == TransformationMode::ROTATE)
        rotate({static_cast<int>(x), static_cast<int>(y)});

    if (_mode == TransformationMode::TRANSLATE)
        translate({static_cast<int>(x), static_cast<int>(y)});

    signal_positionChanged.emit(_currentPosition.x(), _currentPosition.y(), _currentPosition.z());
    signal_changed.emit();
}
