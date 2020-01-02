//
// Created by jackcamp on 12/26/19.
//

#include "Viewport.h"
#include "ColoredDrawable.h"

using namespace Magnum;

Viewport::Viewport(const Arguments &arguments) :
        Platform::Application(
                arguments,
                Configuration{}.setTitle("mini 2020")
        ) {


    // Configuring CLI Input
    auto config = Config(arguments.argc, arguments.argv);
    auto logger = config.logger;


    // Creating shared Meshes and Shaders
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sphereMesh = MeshTools::compile(Primitives::uvSphereSolid(16, 32));

    shader = Shaders::Phong{};
    shader
            .setLightPosition({00.0f, 0.0f, -250.0f});


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



    // Adding the bodies
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (Body b : config.bodies) {

        Matrix4 transformation = Matrix4::translation({b.getPosition().x, b.getPosition().y, b.getPosition().z});

        auto sphereObject = new Object3D(&manipulator);
        sphereObject->setTransformation(transformation);

        auto color = Color4::fromSrgb(Vector3(b.getColor().r, b.getColor().g, b.getColor().b), b.getColor().s / 2);

        auto scaling = Matrix4::scaling({10, 10, 10});

        new ColoredDrawable{*sphereObject, drawables, sphereMesh, shader, color, scaling};
    }


    // Adding a sphere
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*Matrix4 sphereTransformation;
    sphereTransformation =
            Matrix4::translation(Vector3::zAxis(-10.0f));

    auto sphereObject = new Object3D(&manipulator);
    sphereObject->setTransformation(sphereTransformation);

    new ColoredDrawable{*sphereObject, drawables, sphereMesh, shader};*/

}

void Viewport::drawEvent() {

    GL::defaultFramebuffer.clear(
            GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    camera->draw(drawables);

    swapBuffers();
}