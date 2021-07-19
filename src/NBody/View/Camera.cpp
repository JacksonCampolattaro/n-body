//
// Created by jackcamp on 7/19/21.
//

#include "Camera.h"

using namespace Magnum::Math::Literals;

void NBody::Camera::draw(const NBody::Simulation::Simulation &simulation) {
//    auto view = simulation.view<const NBody::Physics::Position, const NBody::Graphics::Color, const NBody::Graphics::Sphere>();
//    view.each([&](const NBody::Physics::Position &position, const NBody::Graphics::Color &color,
//                  const NBody::Graphics::Sphere &sphere) {
//
//
//        auto transformationMatrix =
//                Matrix4::rotationX(30.0_degf) * Matrix4::rotationY(40.0_degf);
////                Matrix4::translation({position.x, position.y, position.z}) *
////                Matrix4::scaling(Vector3{1.0});
//
//        _shader.setDiffuseColor(0xFFFFFF_rgbf)
//                .setAmbientColor(0xFFFFFF_rgbf)
//                .setTransformationMatrix(transformationMatrix)
//                .setNormalMatrix(transformationMatrix.normalMatrix())
//                .setProjectionMatrix(_projection)
//                .draw(Graphics::Sphere::mesh());
//    });


    auto color = 0xAAAAAA_rgbf;
    auto transformationMatrix =
            Matrix4::rotationX(30.0_degf) * Matrix4::rotationY(40.0_degf);
    Shaders::PhongGL shader{{}, 1};
    shader.setLightPositions({{1.4f, 1.0f, 0.75f, 0.0f}})
            .setDiffuseColor(color)
            .setAmbientColor(Color3::fromHsv({color.hue(), 1.0f, 0.3f}))
            .setTransformationMatrix(transformationMatrix)
            .setNormalMatrix(transformationMatrix.normalMatrix())
            .setProjectionMatrix(_projection)
            .draw(Graphics::Sphere::mesh());

    std::cout << "x" << std::endl;
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
