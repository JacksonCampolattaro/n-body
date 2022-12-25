//
// Created by Jackson Campolattaro on 7/24/22.
//

#include "InstancedPhongRenderer.h"

void NBody::InstancedPhongRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix, const NBody::Simulation &simulation) {

    // Allocate the shader & mesh, if this hasn't been done before
    if (!_shader || !_mesh) {

        _shader = Shaders::PhongGL{
                Shaders::PhongGL::Flag::NoSpecular |
                Shaders::PhongGL::Flag::VertexColor |
                Shaders::PhongGL::Flag::InstancedTransformation
        };
        _mesh = NBody::Graphics::Sphere::mesh();
    }

    auto &shader = *_shader;
    auto &mesh = *_mesh;

    auto spheresView = simulation.view<const NBody::Physics::Position, const NBody::Graphics::Color, const NBody::Graphics::Sphere>();

    Containers::Array<SphereInstanceData> instanceData{NoInit, spheresView.size_hint()};

    std::size_t i = 0;
    spheresView.each([&](
            const NBody::Physics::Position &position,
            const NBody::Graphics::Color &color,
            const NBody::Graphics::Sphere &sphere) {

        instanceData[i].transformationMatrix =
                Matrix4::translation({position.x, position.y, position.z}) *
                Matrix4::scaling(Vector3{sphere.radius()});
        instanceData[i].normalMatrix =
                instanceData[i].transformationMatrix.normalMatrix();
        instanceData[i].color = Color3{color.r(), color.g(), color.b()};

        i++;
    });

    auto instanceBuffer = GL::Buffer{};
    mesh.addVertexBufferInstanced(instanceBuffer, 1, 0,
                                   Shaders::PhongGL::TransformationMatrix{},
                                   Shaders::PhongGL::NormalMatrix{},
                                   Shaders::PhongGL::Color3{});
    mesh.setInstanceCount((int) instanceData.size());

    instanceBuffer.setData(instanceData, GL::BufferUsage::DynamicDraw);

    shader
            .setAmbientColor(Color3{0.2, 0.2, 0.2})
            .setProjectionMatrix(projectionMatrix)
            .setTransformationMatrix(transformationMatrix)
            .setNormalMatrix(transformationMatrix.normalMatrix())
            .draw(mesh);
}
