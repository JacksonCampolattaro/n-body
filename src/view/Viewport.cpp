//
// Created by jackcamp on 12/26/19.
//

#include "Viewport.h"

using namespace Magnum;

Viewport::Viewport(const Arguments &arguments) :
        Magnum::Platform::Application(
                arguments,
                Configuration{}.setTitle("MINI 2020")
        ) {

    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);

    const Magnum::Trade::MeshData3D cube = Magnum::Primitives::cubeSolid();

    Magnum::GL::Buffer vertices;
    vertices.setData(Magnum::MeshTools::interleave(cube.positions(0), cube.normals(0)));

    Magnum::Containers::Array<char> indexData;
    Magnum::MeshIndexType indexType;
    Magnum::UnsignedInt indexStart, indexEnd;
    std::tie(indexData, indexType, indexStart, indexEnd) =
            Magnum::MeshTools::compressIndices(cube.indices());
    Magnum::GL::Buffer indices;
    indices.setData(indexData);

    _mesh.setPrimitive(cube.primitive())
            .setCount(cube.indices().size())
            .addVertexBuffer(std::move(vertices), 0, Magnum::Shaders::Phong::Position{},
                             Magnum::Shaders::Phong::Normal{})
            .setIndexBuffer(std::move(indices), 0, indexType, indexStart, indexEnd);

    _transformation =
            Magnum::Matrix4::rotationX(Magnum::Math::Deg<float>(30.0))*Magnum::Matrix4::rotationY(Magnum::Math::Deg<float>(40.0));
    _projection =
            Magnum::Matrix4::perspectiveProjection(
                    Magnum::Math::Deg<float>(35.0), Magnum::Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f)*
            Magnum::Matrix4::translation(Magnum::Vector3::zAxis(-10.0f));
    _color = Magnum::Color3::fromHsv({Magnum::Math::Deg<float>(35.0), 1.0f, 1.0f});
}

void Viewport::drawEvent() {

    GL::defaultFramebuffer.clear(
            GL::FramebufferClear::Color|GL::FramebufferClear::Depth);

    _shader.setLightPosition({7.0f, 5.0f, 2.5f})
            .setLightColor(Color3{1.0f})
            .setDiffuseColor(_color)
            .setAmbientColor(Color3::fromHsv({_color.hue(), 1.0f, 0.3f}))
            .setTransformationMatrix(_transformation)
            .setNormalMatrix(_transformation.rotationNormalized())
            .setProjectionMatrix(_projection);
    _mesh.draw(_shader);

    swapBuffers();
}
