//
// Created by Jackson Campolattaro on 8/25/22.
//

#include "ArcBallControllableCamera.h"

#include "Renderers/SolverRenderer.h"

NBody::ArcBallControllableCamera::ArcBallControllableCamera(const Vector3 &cameraPosition,
                                                            const Vector3 &viewCenter,
                                                            Deg fov) :
        _arcBall(cameraPosition, viewCenter, Vector3::yAxis(), fov) {

    _arcBall.setLagging(0.8);
    _direction = _arcBall.getDirection();
}

float NBody::ArcBallControllableCamera::getZoom() const {
    return _arcBall.getZoom();
}

void NBody::ArcBallControllableCamera::setZoom(float zoom) {
    _arcBall.setZoom(zoom);
    signal_changed().emit();
}

Vector3 NBody::ArcBallControllableCamera::getPosition() const {
    return _arcBall.getPosition();
}

void NBody::ArcBallControllableCamera::setPosition(const Vector3 &position) {
    _arcBall.setPosition(position);
    signal_changed().emit();
}

Vector3 NBody::ArcBallControllableCamera::getDirection() const {
    return _direction;
}

void NBody::ArcBallControllableCamera::draw(const NBody::Simulation &simulation, GL::Framebuffer &framebuffer) {
    std::scoped_lock l(simulation.mutex);

    if (_arcBall.updateTransformation())
        signal_changed().emit();

    framebuffer.bind();

    // Set the background color
    GL::Renderer::setClearColor(getBackgroundColor());

    // Reset color and depth buffers
    framebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    renderer().draw(
            _arcBall.viewMatrix(),
            _arcBall.perspectiveProjection(Vector2{framebuffer.viewport().size()}.aspectRatio()),
            simulation
    );
}

void NBody::ArcBallControllableCamera::draw(const NBody::Simulation &simulation, const NBody::Solver &solver,
                                            GL::Framebuffer &framebuffer) {
    NBody::SolverRenderer::draw(
            _arcBall.viewMatrix(),
            _arcBall.perspectiveProjection(Vector2{framebuffer.viewport().size()}.aspectRatio()),
            simulation,
            solver
    );
}

void NBody::ArcBallControllableCamera::drawHUD(const Vector2i &windowDimensions) {
    // todo: The arcball camera could draw a heads-up-display while it's moving
}

void NBody::ArcBallControllableCamera::scroll(double dx, double dy) {
    spdlog::trace("Scroll signal received: dx={}, dy={}", dx, dy);
    _arcBall.zoom((float) -10.0f * dy);
    signal_changed().emit();
}

void NBody::ArcBallControllableCamera::leftMousePress(const Vector2 &mousePos) {
    spdlog::trace("Left click signal received: x={}, y={}", mousePos.x(), mousePos.y());
    _arcBall.initTransformation(mousePos);
    _mode = TransformationMode::ROTATE;
}

void NBody::ArcBallControllableCamera::rightMousePress(const Vector2 &mousePos) {
    spdlog::trace("Right click signal received: x={}, y={}", mousePos.x(), mousePos.y());
    _arcBall.initTransformation(mousePos);
    _mode = TransformationMode::TRANSLATE;
}

void NBody::ArcBallControllableCamera::mouseRelease() {
    spdlog::trace("Mouse button released");
    _mode = TransformationMode::NONE;
    signal_changed().emit();
}

void NBody::ArcBallControllableCamera::mouseMotion(const Vector2 &mousePos) {

    if (_mode == TransformationMode::NONE) {
        return;
    }

    if (_mode == TransformationMode::ROTATE) {
        _arcBall.rotate(mousePos);
        _direction = _arcBall.getDirection();
        signal_changed().emit();
    }

    if (_mode == TransformationMode::TRANSLATE) {
        _arcBall.translate(mousePos);
        signal_changed().emit();
    }

}
