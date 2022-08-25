//
// Created by jackcamp on 8/17/21.
//

#include "GtkmmArcBallCamera.h"

void NBody::GtkmmArcBallCamera::setPosition(float x, float y, float z) {

    spdlog::trace("Moving the camera to ({}, {}, {})", x, y, z);

    _targetPosition = {x, y, z};

    signal_changed.emit();
    signal_positionChanged.emit(x, y, z);
}

void NBody::GtkmmArcBallCamera::setZoom(float z) {
    _targetZooming = z;
    signal_zoomChanged.emit(_targetZooming);
    signal_changed.emit();
}

void NBody::GtkmmArcBallCamera::setBackgroundColor(const Gdk::RGBA &color) {
    _backgroundColor = {color.get_red(), color.get_green(), color.get_blue()};
    signal_changed.emit();
}

bool NBody::GtkmmArcBallCamera::on_scroll(double dx, double dy) {
    spdlog::trace("Scroll signal received: dx={}, dy={}", dx, dy);

    zoom((float) -dy);
    signal_zoomChanged.emit(_targetZooming);
    signal_changed.emit();
    return false;
}

void NBody::GtkmmArcBallCamera::on_leftButtonPress(Vector2 mousePosNDC) {
    spdlog::trace("Left click signal received: x={}, y={}", mousePosNDC.x(), mousePosNDC.y());

    initTransformation(mousePosNDC);
    _mode = TransformationMode::ROTATE;
}

void NBody::GtkmmArcBallCamera::on_middleButtonPress(Vector2 mousePosNDC) {
    spdlog::trace("Middle click signal received: x={}, y={}", mousePosNDC.x(), mousePosNDC.y());

    initTransformation(mousePosNDC);
    _mode = TransformationMode::TRANSLATE;
}

void NBody::GtkmmArcBallCamera::on_buttonRelease() {
    spdlog::trace("Mouse button released");

    _mode = TransformationMode::NONE;
    signal_changed.emit();
}

void NBody::GtkmmArcBallCamera::on_mouseMotion(Vector2 mousePosNDC) {

    if (_mode == TransformationMode::NONE) {
        return;
    }

    if (_mode == TransformationMode::ROTATE) {
        rotate(mousePosNDC);
        Vector3 direction = _currentQRotation.toMatrix() * Vector3::zAxis();
        signal_directionChanged.emit(direction.x(), direction.y(), direction.z());
    }

    if (_mode == TransformationMode::TRANSLATE) {
        translate(mousePosNDC);
        signal_positionChanged.emit(_currentPosition.x(), _currentPosition.y(), _currentPosition.z());
    }

    signal_changed.emit();
}

