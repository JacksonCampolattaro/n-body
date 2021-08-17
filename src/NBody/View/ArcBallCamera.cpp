//
// Created by jackcamp on 8/16/21.
//

#include "ArcBallCamera.h"

void NBody::ArcBallCamera::draw(const NBody::Simulation &simulation) {

    draw(simulation.view<const NBody::Physics::Position, const NBody::Graphics::Color, const NBody::Graphics::Sphere>());
}

void NBody::ArcBallCamera::draw(
        entt::basic_view<entt::entity, entt::exclude_t<>, const NBody::Physics::Position, const NBody::Graphics::Color, const NBody::Graphics::Sphere> view) {

    auto mesh = Graphics::Sphere::mesh();

    Shaders::PhongGL shader{};

    view.each([&](
            const NBody::Physics::Position &position,
            const NBody::Graphics::Color &color,
            const NBody::Graphics::Sphere &sphere) {

        // todo
        auto transformationMatrix =
                viewMatrix() *
                Matrix4::translation({position.x, position.y, position.z}) *
                Matrix4::scaling(Vector3{sphere.radius()});

        shader.setDiffuseColor(color)
                .setAmbientColor(Color3::fromHsv({color.hue(), 1.0f, 0.5f}))
                .setTransformationMatrix(transformationMatrix)
                .setNormalMatrix(transformationMatrix.normalMatrix())
                .setProjectionMatrix(perspectiveProjection())
                .draw(mesh);
    });
}

void NBody::ArcBallCamera::draw() {

    auto mesh = MeshTools::compile(Primitives::cubeSolid());

    auto shader = Shaders::PhongGL{};
    shader
            .setTransformationMatrix(viewMatrix())
            .setNormalMatrix(viewMatrix().normalMatrix())
            .setProjectionMatrix(perspectiveProjection())
            .draw(mesh);
}
