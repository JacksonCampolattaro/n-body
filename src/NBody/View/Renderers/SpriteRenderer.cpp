//
// Created by Jackson Campolattaro on 1/25/23.
//

#include "SpriteRenderer.h"

void NBody::SpriteRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) {

    auto shader = Shaders::FlatGL3D{
            Shaders::FlatGL3D::Flag::InstancedTransformation |
            Shaders::FlatGL3D::Flag::VertexColor
    };
    auto mesh = MeshTools::compile(Primitives::circle3DSolid(64));
    GL::Buffer circleInstanceBuffer{};
    Containers::Array<CircleInstanceData> circleInstanceData;
    mesh.addVertexBufferInstanced(circleInstanceBuffer, 1, 0,
                                  Shaders::FlatGL3D::TransformationMatrix{},
                                  Shaders::FlatGL3D::Color4{});

    std::scoped_lock l(_simulation.mutex);
    auto velocitiesView = _simulation.view<
            const NBody::Physics::Position,
            const NBody::Graphics::Sphere,
            const NBody::Graphics::Color>();

    velocitiesView.each([&](
            const NBody::Physics::Position &position,
            const NBody::Graphics::Sphere &sphere,
            const NBody::Graphics::Color &color) {

        auto individualTransformationMatrix =
                transformationMatrix *
                Matrix4::translation({position.x, position.y, position.z}) *
                Matrix4::scaling(Vector3{sphere.radius()}) *
                Matrix4::from(transformationMatrix.rotation().inverted(), {0.0f, 0.0f, 0.0f});

        Containers::arrayAppend(circleInstanceData, InPlaceInit,
                                individualTransformationMatrix,
                                Color4{color.r(), color.g(), color.b()});
    });

    circleInstanceBuffer.setData(circleInstanceData, GL::BufferUsage::DynamicDraw);
    mesh.setInstanceCount((int) circleInstanceData.size());

    shader
            .setTransformationProjectionMatrix(projectionMatrix)
            .draw(mesh);

}
