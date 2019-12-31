//
// Created by jackcamp on 12/26/19.
//

#include "Viewport.h"

using namespace Magnum;

Viewport::Viewport(const Arguments &arguments) :
        Platform::Application(
                arguments,
                Configuration{}.setTitle("MINI 2020")
        ) {


    // Configuring CLI Input
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Config config(arguments.argc, arguments.argv);


    // Configuring logging
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    auto logger = config.logger();

    // Loading from XML files
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Loading Bodies
    std::ifstream bodiesInputStream(config.bodyArchivePath);
    if (!bodiesInputStream.is_open()) {
        logger->error("Failed to load body file at path \"{}\"", config.bodyArchivePath);
    }
    cereal::XMLInputArchive bodiesInputArchive(bodiesInputStream);
    auto bodies = BodyList();
    bodiesInputArchive(bodies);

    // Loading Physics
    std::ifstream physicsInputStream(config.physicsArchivePath);
    if (!bodiesInputStream.is_open()) {
        logger->error("Failed to load physics file at path \"{}\"", config.physicsArchivePath);
    }
    cereal::XMLInputArchive physicsInputArchive(physicsInputStream);
    auto physics = PhysicsContext();
    physicsInputArchive(physics);



    // Adding a sphere to the view
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    const Trade::MeshData3D sphere = Primitives::uvSphereSolid(50, 50);

    GL::Buffer vertices;
    vertices.setData(MeshTools::interleave(sphere.positions(0), sphere.normals(0)));

    Containers::Array<char> indexData;
    MeshIndexType indexType;
    UnsignedInt indexStart, indexEnd;
    std::tie(indexData, indexType, indexStart, indexEnd) =
            MeshTools::compressIndices(sphere.indices());
    GL::Buffer indices;
    indices.setData(indexData);

    _mesh.setPrimitive(sphere.primitive())
            .setCount(sphere.indices().size())
            .addVertexBuffer(std::move(vertices), 0, Shaders::Phong::Position{},
                             Shaders::Phong::Normal{})
            .setIndexBuffer(std::move(indices), 0, indexType, indexStart, indexEnd);

    _transformation =
            Matrix4::rotationX(Math::Deg<float>(30.0)) *
            Matrix4::rotationY(Math::Deg<float>(40.0));

    _projection =
            Matrix4::perspectiveProjection(
                    Math::Deg<float>(35.0), Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f) *
            Matrix4::translation(Vector3::zAxis(-10.0f));

    _color = Color3::fromHsv({Math::Deg<float>(35.0), 1.0f, 1.0f});
}

void Viewport::drawEvent() {

    GL::defaultFramebuffer.clear(
            GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

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
