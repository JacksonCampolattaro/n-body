//
// Created by Jackson Campolattaro on 7/24/22.
//

#include "PhongRenderer.h"

void NBody::PhongRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix, const NBody::Simulation &simulation) {
    auto shader = Shaders::PhongGL{Shaders::PhongGL::Flag::NoSpecular};
    auto mesh = NBody::Graphics::Sphere::mesh();

    auto spheresView = simulation.view<const NBody::Physics::Position, const NBody::Graphics::Color, const NBody::Graphics::Sphere>();

    spheresView.each([&](
            const NBody::Physics::Position &position,
            const NBody::Graphics::Color &color,
            const NBody::Graphics::Sphere &sphere) {

        auto individualTransformationMatrix =
                transformationMatrix *
                Matrix4::translation({position.x, position.y, position.z}) *
                Matrix4::scaling(Vector3{sphere.radius()});

        shader
                .setDiffuseColor(color)
                .setAmbientColor(Color3::fromHsv({color.hue(), color.saturation(), color.value() * 0.7f}))
                .setTransformationMatrix(individualTransformationMatrix)
                .setNormalMatrix(individualTransformationMatrix.normalMatrix())
                .setProjectionMatrix(projectionMatrix)
                .draw(mesh);
    });
}
