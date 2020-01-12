//
// Created by jackcamp on 12/26/19.
//

#include "Viewport.h"
#include "../model/calculation/Solver.h"
#include "../model/calculation/BarnesHut/BarnesHutSolver.h"

using namespace Magnum;

Viewport::Viewport(const Arguments &arguments) :
        Platform::Application(
                arguments,
                Configuration{}.setTitle("mini 2020")
        ) {


    // Configuring CLI Input
    config = new Config(arguments.argc, arguments.argv);


    // Creating shared Meshes and Shaders
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sphereMesh = MeshTools::compile(Primitives::uvSphereSolid(16, 32));

    shader = Shaders::Phong{};
    shader
            .setShininess(50)
            .setLightPosition({0.0f, 0.0f, -50.0f});


    // Building a scene
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Camera
    cameraObject
            .setParent(&scene)
            .translate(Vector3::zAxis(500.0f));
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

    for (Body b : config->bodies) {

        auto bodyObject = new BodyObject(&manipulator, &b);

        auto color = Color4::fromSrgb(Vector3(b.getColor().r, b.getColor().g, b.getColor().b), b.getColor().s / 2);
        auto scaling = Matrix4::scaling(Vector3(b.getRadius()));
        new ColoredDrawable{*bodyObject, drawables, sphereMesh, shader, color, scaling};

        bodyObjectList.push_back(bodyObject);
        bodyObject->update();
    }

}

void Viewport::drawEvent() {
    config->logger->info("DrawEvent Triggered");

    GL::defaultFramebuffer.clear(
            GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    camera->draw(drawables);
    config->logger->info("Finished Drawing");

    swapBuffers();
    config->logger->info("Swapped Buffers");


    config->solver->solve(&config->bodies, &config->physics);
    config->logger->info("Finished calculations");

    for (auto b : bodyObjectList) {
        b->update();
    }
    config->logger->info("Finished Updating buffers");

    ///manipulator.setDirty();

    redraw();
}
