//
// Created by jackcamp on 7/19/21.
//

#include "Camera.h"

#include <entt/entity/view.hpp>

using namespace Magnum::Math::Literals;

void NBody::Camera::draw(const NBody::Simulation &simulation) {

    draw();
//    draw(simulation.view<
//            const NBody::Physics::Position,
//            const NBody::Graphics::Color,
//            const NBody::Graphics::Sphere>());
}

void NBody::Camera::draw(entt::basic_view<entt::entity, entt::exclude_t<>,
        const NBody::Physics::Position,
        const NBody::Graphics::Color,
        const NBody::Graphics::Sphere> view) {

    auto mesh = Graphics::Sphere::mesh();

    Shaders::PhongGL shader{{}, 1};
//    shader.setLightPositions({{24.0f, 50.0f, -50, 0.0f}})
//            .setProjectionMatrix(_projection * _translation);

    view.each([&](
            const NBody::Physics::Position &position,
            const NBody::Graphics::Color &color,
            const NBody::Graphics::Sphere &sphere) {

        auto transformationMatrix =
                Matrix4::translation({position.x, position.y, position.z}) *
                Matrix4::scaling(Vector3{sphere.radius()});

        shader.setDiffuseColor(color)
                .setAmbientColor(Color3::fromHsv({color.hue(), 1.0f, 0.5f}))
                .setTransformationMatrix(transformationMatrix)
                .setNormalMatrix(transformationMatrix.normalMatrix())
                .draw(mesh);
    });
}

void NBody::Camera::draw() {
//
//    auto mesh = MeshTools::compile(Primitives::cubeSolid());
//    auto projection =
//            Matrix4::perspectiveProjection(35.0_degf, _windowDimensions.aspectRatio(), 0.01f, 100.0f) *
//            _translation;
//    auto color = Color3::fromHsv({35.0_degf, 1.0, 1.0});
//
//    auto shader = Shaders::PhongGL{{}, 1};
//    shader.setLightPositions({{24.0f, 50.0f, -50, 0.0f}})
//            .setDiffuseColor(color)
//            .setAmbientColor(Color3::fromHsv({color.hue(), 1.0f, 0.3f}))
//            .setTransformationMatrix(_transformation)
//            .setNormalMatrix(_transformation.normalMatrix())
//            .setProjectionMatrix(projection)
//            .draw(mesh);

}

//void NBody::Camera::draw(const NBody::Simulation::Simulation &simulation) {
//
//    _sphereInstanceData =
//            Containers::Array<Graphics::Sphere::InstanceData>{NoInit, simulation.size<Graphics::Sphere>()};
//
//    std::size_t i = 0;
//    auto view = simulation.view<const NBody::Physics::Position, const NBody::Graphics::Color, const NBody::Graphics::Sphere>();
//    view.each([&](const NBody::Physics::Position &position, const NBody::Graphics::Color &color,
//                  const NBody::Graphics::Sphere &sphere) {
//
//        _sphereInstanceData[i].transformationMatrix =
//                Matrix4::translation({position.x, position.y, position.z}) *
//                Matrix4::scaling(Vector3{1.0});
//        _sphereInstanceData[i].normalMatrix =
//                _sphereInstanceData[i].transformationMatrix.normalMatrix();
//        _sphereInstanceData[i].color =
//                Color3{0.9};
//
//        i++;
//    });
//
//    _sphereInstanceBuffer = GL::Buffer{};
//    auto _sphereMesh = Graphics::Sphere::mesh();
//    _sphereMesh.addVertexBufferInstanced(
//            _sphereInstanceBuffer, 1, 0,
//            Shaders::PhongGL::TransformationMatrix{},
//            Shaders::PhongGL::NormalMatrix{},
//            Shaders::PhongGL::Color3{}
//    );
//    _sphereMesh.setInstanceCount(_sphereInstanceData.size());
//
//    _shader = Shaders::PhongGL{
//            Shaders::PhongGL::Flag::VertexColor |
//            Shaders::PhongGL::Flag::InstancedTransformation
//    };
//    _shader.setProjectionMatrix(_projection)
//            .draw(_sphereMesh);
//}
