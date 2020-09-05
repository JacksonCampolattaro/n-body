//
// Created by jackcamp on 9/1/20.
//

#include "SimulationViewport.h"

#include <spdlog/spdlog.h>

using namespace Magnum;
using namespace Math::Literals;

View::SimulationViewport::SimulationViewport() :
        Gtk::GLArea(),
        _context(NoCreate) {

    spdlog::trace("MagnumViewport constructor invoked");

    set_size_request(1000, 1000);

    // Automatically re-render everything each time it needs to be redrawn
    set_auto_render();

    // Set desired OpenGL version
    set_required_version(4, 5);

    // Connecting signals to their respective handlers
    signal_realize().connect(sigc::mem_fun(this, &SimulationViewport::onRealize));
    signal_render().connect(sigc::mem_fun(this, &SimulationViewport::onRender));
    signal_resize().connect(sigc::mem_fun(this, &SimulationViewport::onResize));
    signal_unrealize().connect(sigc::mem_fun(this, &SimulationViewport::onUnrealize));

}

void View::SimulationViewport::draw(const std::deque<Model::Drawable::Drawable> &drawables) {

    _sphereInstanceData = Containers::Array<SphereInstanceData>{Containers::NoInit, drawables.size()};
    for (int i = 0; i < drawables.size(); ++i) {

        auto position = *drawables[i]._position;
        auto color = drawables[i]._color;

        _sphereInstanceData[i].transformation =
                Matrix4::translation({position.x, position.y, position.z})
                * Matrix4::scaling(Vector3{drawables[i]._radius});

        _sphereInstanceData[i].normal =
                _sphereInstanceData[i].transformation.normalMatrix();

        _sphereInstanceData[i].color =
                color;
    }
    _sphereMesh.setInstanceCount(drawables.size());

    queue_render();
}

void View::SimulationViewport::onRealize() {

    spdlog::trace("SimulationViewport onRealize invoked");

    // Make the OpenGL context current, and then configure it
    make_current();
    _context.create();

    // Configure the graphics features
    set_has_depth_buffer();
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    _shader = Shaders::Phong{
            Shaders::Phong::Flag::VertexColor |
            Shaders::Phong::Flag::InstancedTransformation
    };
    _shader
            .setAmbientColor(0x505050_rgbf)
            .setSpecularColor(0x000000_rgbf)
            .setLightPosition({0.0, 20.0, 35.0});

    _sphereInstanceBuffer = GL::Buffer{};
    _sphereMesh = MeshTools::compile(Primitives::icosphereSolid(3));
    _sphereMesh.addVertexBufferInstanced(_sphereInstanceBuffer, 1, 0,
                                         Shaders::Phong::TransformationMatrix{},
                                         Shaders::Phong::NormalMatrix{},
                                         Shaders::Phong::Color3{});

}

bool View::SimulationViewport::onRender(const Glib::RefPtr<Gdk::GLContext> &context) {

    spdlog::trace("SimulationViewport onRender invoked");

    // Reset state, so Gtkmm's defaults won't interfere with Magnum
    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    // Get the ID of the current framebuffer
    GLint framebufferID;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);

    // Attach Magnum's framebuffer manager to the framebuffer provided by Gtkmm
    auto framebuffer =
            GL::Framebuffer::wrap(framebufferID, {{},
                                                  {get_width(), get_height()}});

    // Clear
    framebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);


    _projection =
            Matrix4::perspectiveProjection(
                    35.0_degf, _aspectRatio, 0.01f, 100.0f) *
            Matrix4::translation(Vector3::zAxis(-35.0f));

    _sphereInstanceBuffer.setData(_sphereInstanceData, GL::BufferUsage::DynamicDraw);
    _shader.setProjectionMatrix(_projection)
            .draw(_sphereMesh);

    // Undo Magnum's changes to the graphics state
    Magnum::GL::Context::current().resetState(GL::Context::State::EnterExternal);;

    signal_render_complete.emit();
    return false;
}

void View::SimulationViewport::onResize(int width, int height) {

    spdlog::trace("SimulationViewport onResize invoked");

    _aspectRatio = (float) width / (float) height;
}

void View::SimulationViewport::onUnrealize() {

    spdlog::trace("SimulationViewport onUnrealize invoked");

    // TODO
}

