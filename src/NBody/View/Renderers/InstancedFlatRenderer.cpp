//
// Created by Jackson Campolattaro on 7/25/22.
//

#include "InstancedFlatRenderer.h"

void NBody::InstancedFlatRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) {

    auto shader = Shaders::FlatGL3D{
            Shaders::FlatGL3D::Flag::InstancedTransformation |
            Shaders::FlatGL3D::Flag::VertexColor
    };
    auto mesh = NBody::Graphics::Sphere::mesh();

    std::scoped_lock l(_simulation.mutex);
    auto spheresView = _simulation.view<const NBody::Physics::Position, const NBody::Graphics::Color, const NBody::Graphics::Sphere>();

    Containers::Array<SphereInstanceData> instanceData{NoInit, spheresView.size_hint()};

    std::size_t i = 0;
    spheresView.each([&](
            const NBody::Physics::Position &position,
            const NBody::Graphics::Color &color,
            const NBody::Graphics::Sphere &sphere) {

        instanceData[i].transformationMatrix =
                Matrix4::translation({position.x, position.y, position.z}) *
                Matrix4::scaling(Vector3{sphere.radius()});
        instanceData[i].color = Color3{color.r(), color.g(), color.b()};

        i++;
    });

    auto instanceBuffer = GL::Buffer{};
    instanceBuffer.setData(instanceData, GL::BufferUsage::DynamicDraw);

    mesh
            .setInstanceCount((int) instanceData.size())
            .addVertexBufferInstanced(instanceBuffer, 1, 0,
                                      Shaders::FlatGL3D::TransformationMatrix{},
                                      Shaders::FlatGL3D::Color3{}
            );

    shader
            .setTransformationProjectionMatrix(projectionMatrix * transformationMatrix)
            .draw(mesh);
}
