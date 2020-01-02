//
// Created by jackcamp on 12/26/19.
//

#include "Viewport.h"
#include "ColoredDrawable.h"

using namespace Magnum;

Viewport::Viewport(const Arguments &arguments) :
        Platform::Application(
                arguments,
                Configuration{}.setTitle("MINI 2020")
        ) {


    // Configuring CLI Input
    auto config = Config(arguments.argc, arguments.argv);
    auto logger = config.logger;



    // Creating shared Meshes and Shaders
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sphereMesh = MeshTools::compile(Primitives::uvSphereSolid(16, 32));

    shader = Shaders::Phong{};
    shader
            .setAmbientColor(0x111111_rgbf)
            .setSpecularColor(0x330000_rgbf)
            .setLightPosition({10.0f, 15.0f, 5.0f});


    // Building a scene
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Camera
    cameraObject
            .setParent(&scene)
            .translate(Vector3::zAxis(5.0f));
    camera = new SceneGraph::Camera3D(cameraObject);
    (*camera)
            .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
            .setProjectionMatrix(Matrix4::perspectiveProjection(35.0_degf, 1.0f, 0.01f, 1000.0f))
            .setViewport(GL::defaultFramebuffer.viewport().size());

    // Root Object
    manipulator.setParent(&scene);


    // Configuring Universal Shader properties
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Enables occlusion based on distance
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);

    // Improves performance by not drawing faces which aren't visible
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    // Setting shader
    /*shader
            .setAmbientColor(0x111111_rgbf)
            .setSpecularColor(0xffffff_rgbf)
            .setShininess(80.0f);*/
    /*shader.setLightPosition({7.0f, 5.0f, 2.5f})
            .setLightColor(Color3{1.0f})
            .setAmbientColor(0x111111_rgbf)
            .setSpecularColor(0xffffff_rgbf)
            .setNormalMatrix(_transformation.rotationNormalized())
            .setShininess(80.0f);*/

    // Adding a sphere
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Matrix4 sphereTransformation;
    sphereTransformation =
            Matrix4::translation(Vector3::zAxis(-10.0f));

    auto sphereObject = new Object3D(&manipulator);
    sphereObject->setTransformation(sphereTransformation);

    new ColoredDrawable{*sphereObject, shader, sphereMesh, 0x111111_rgbf, drawables};


    /*

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

     */
}

void Viewport::drawEvent() {

    GL::defaultFramebuffer.clear(
            GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    camera->draw(drawables);

    swapBuffers();

    /*

    _shader.setLightPosition({7.0f, 5.0f, 2.5f})
            .setLightColor(Color3{1.0f})
            .setDiffuseColor(_color)
            .setAmbientColor(Color3::fromHsv({_color.hue(), 1.0f, 0.3f}))
            .setTransformationMatrix(_transformation)
            .setNormalMatrix(_transformation.rotationNormalized())
            .setProjectionMatrix(_projection);

    _mesh.draw(_shader);

     */
}
