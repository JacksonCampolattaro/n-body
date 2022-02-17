//
// Created by jackcamp on 8/16/21.
//

#include "ArcBallCamera.h"

void NBody::ArcBallCamera::draw(const NBody::Simulation &simulation) {

    _draw(simulation.view<const NBody::Physics::Position, const NBody::Graphics::Color, const NBody::Graphics::Sphere>());
}

void NBody::ArcBallCamera::draw(
        entt::basic_view<entt::entity, entt::exclude_t<>, const NBody::Physics::Position, const NBody::Graphics::Color, const NBody::Graphics::Sphere> view) {

    static auto mesh = Graphics::Sphere::mesh();

    Shaders::PhongGL shader{Shaders::PhongGL::Flag::NoSpecular};

    view.each([&](
            const NBody::Physics::Position &position,
            const NBody::Graphics::Color &color,
            const NBody::Graphics::Sphere &sphere) {

        auto transformationMatrix =
                viewMatrix() *
                Matrix4::translation({position.x, position.y, position.z}) *
                Matrix4::scaling(Vector3{sphere.radius()});

        shader
                .setDiffuseColor(color)
                .setAmbientColor(Color3::fromHsv({color.hue(), 1.0f, 0.7f}))
                .setTransformationMatrix(transformationMatrix)
                .setNormalMatrix(transformationMatrix.normalMatrix())
                .setProjectionMatrix(perspectiveProjection())
                .draw(mesh);
    });
}

void NBody::ArcBallCamera::_draw(entt::basic_view<entt::entity, entt::exclude_t<>,
        const NBody::Physics::Position,
        const NBody::Graphics::Color,
        const NBody::Graphics::Sphere> view) {

    Containers::Array<SphereInstanceData> instanceData{NoInit, view.size_hint()};

    std::size_t i = 0;
    view.each([&](
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

    Shaders::PhongGL shader = Shaders::PhongGL{
            Shaders::PhongGL::Flag::NoSpecular |
            Shaders::PhongGL::Flag::VertexColor |
            Shaders::PhongGL::Flag::InstancedTransformation
    };
    auto instanceBuffer = GL::Buffer{};
    GL::Mesh mesh = NBody::Graphics::Sphere::mesh(); // todo: creating this every time is wasteful
    mesh.addVertexBufferInstanced(instanceBuffer, 1, 0,
                                  Shaders::PhongGL::TransformationMatrix{},
                                  Shaders::PhongGL::NormalMatrix{},
                                  Shaders::PhongGL::Color3{});
    mesh.setInstanceCount(instanceData.size());

    instanceBuffer.setData(instanceData, GL::BufferUsage::DynamicDraw);
    shader
            .setAmbientColor(Color3{0.2, 0.2, 0.2})
            .setProjectionMatrix(perspectiveProjection())
            .setTransformationMatrix(viewMatrix())
            .setNormalMatrix(viewMatrix().normalMatrix())
            .draw(mesh);
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
