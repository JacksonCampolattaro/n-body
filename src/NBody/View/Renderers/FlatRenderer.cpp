//
// Created by Jackson Campolattaro on 7/25/22.
//

#include "FlatRenderer.h"

void NBody::FlatRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) {

    auto shader = Shaders::FlatGL3D{};
    auto mesh = NBody::Graphics::Sphere::mesh();

    std::scoped_lock l(_simulation.mutex);
    auto spheresView = _simulation.view<const NBody::Physics::Position, const NBody::Graphics::Color, const NBody::Graphics::Sphere>();

    spheresView.each([&](
            const NBody::Physics::Position &position,
            const NBody::Graphics::Color &color,
            const NBody::Graphics::Sphere &sphere) {

        auto individualTransformationMatrix =
                transformationMatrix *
                Matrix4::translation({position.x, position.y, position.z}) *
                Matrix4::scaling(Vector3{sphere.radius()});

        shader
            .setColor(color)
            .setTransformationProjectionMatrix(projectionMatrix * individualTransformationMatrix)
            .draw(mesh);
    });
}
