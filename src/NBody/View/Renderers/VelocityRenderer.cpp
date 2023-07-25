//
// Created by Jackson Campolattaro on 1/25/23.
//

#include "VelocityRenderer.h"

void NBody::VelocityRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) {

    auto shader = Shaders::FlatGL3D{Shaders::FlatGL3D::Configuration{}.setFlags(
            Shaders::FlatGL3D::Flag::InstancedTransformation |
            Shaders::FlatGL3D::Flag::VertexColor
    )};
    auto mesh = MeshTools::compile(Primitives::line3D());
    GL::Buffer lineInstanceBuffer{};
    Containers::Array<LineInstanceData> lineInstanceData;
    mesh.addVertexBufferInstanced(lineInstanceBuffer, 1, 0,
                                  Shaders::FlatGL3D::TransformationMatrix{},
                                  Shaders::FlatGL3D::Color4{});

    std::scoped_lock l(_simulation.mutex);
    auto velocitiesView = _simulation.view<
            const NBody::Physics::Position,
            const NBody::Physics::Velocity,
            const NBody::Graphics::Color>();

    velocitiesView.each([&](
            const NBody::Physics::Position &position,
            const NBody::Physics::Velocity &velocity,
            const NBody::Graphics::Color &color) {

        if (glm::length((glm::vec3) velocity) == 0.0f) return;

        Vector3 relativeRotationAxis = Math::cross(Vector3::xAxis(),
                                                   {velocity.x, velocity.y, velocity.z}).normalized();
        //        if (relativeRotationAxis.length() != 1.0f)
        //            relativeRotationAxis = Math::cross(Vector3::xAxis(),
        //                                               {velocity.x, velocity.y, velocity.z + 100.0f}).normalized();

        Rad relativeRotationAngle{std::acos(Math::dot(Vector3::xAxis(),
                                                      Vector3{velocity.x, velocity.y, velocity.z}.normalized()))};


        auto individualTransformationMatrix =
                transformationMatrix *
                Matrix4::translation({position.x, position.y, position.z}) *
                Matrix4::scaling(Vector3{glm::length((glm::vec3) velocity)}) *
                Matrix4::rotation(relativeRotationAngle, relativeRotationAxis);

        Containers::arrayAppend(lineInstanceData, InPlaceInit,
                                individualTransformationMatrix,
                                Color4{color.r(), color.g(), color.b()});
    });

    lineInstanceBuffer.setData(lineInstanceData, GL::BufferUsage::DynamicDraw);
    mesh.setInstanceCount((int) lineInstanceData.size());

    shader
            .setTransformationProjectionMatrix(projectionMatrix)
            .draw(mesh);
}
