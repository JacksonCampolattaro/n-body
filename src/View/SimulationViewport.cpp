//
// Created by jackcamp on 9/1/20.
//

#include "SimulationViewport.h"

using namespace Magnum;
using namespace Math::Literals;

View::SimulationViewport::SimulationViewport() :
        Gtk::GLArea(),
        _context(NoCreate) {

    spdlog::trace("MagnumViewport constructor invoked");

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

void View::SimulationViewport::attach_simulation(std::shared_ptr<Model::Simulation> simulation) {
    _simulation = simulation;
}

void View::SimulationViewport::onRealize() {

    spdlog::trace("SimulationViewport onRealize invoked");

    // Make the OpenGL context current, and then configure it
    make_current();
    _context.create();

    // Configure the graphics features
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    _sphereMesh = MeshTools::compile(Primitives::icosphereSolid(3));
    _shader = Shaders::Phong();
    _shader
            .setLightPosition({7.0f, 5.0f, 2.5f})
            .setLightColor(Color3{1.0f})
            .setAmbientColor(Color3{0.4f});
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
    framebuffer.clear(GL::FramebufferClear::Color);

    if (_simulation) {

        spdlog::trace("rendering simulation");

        for (auto location : _simulation->_positions) {

            _projection =
                    Matrix4::perspectiveProjection(
                            35.0_degf, _aspectRatio, 0.01f, 100.0f) *
                    Matrix4::translation(Vector3::zAxis(-35.0f));

            auto locationTransformation =
                    Matrix4::translation({location.x, location.y, location.z});

            _shader
                    .setTransformationMatrix(locationTransformation)
                    .setNormalMatrix(locationTransformation.normalMatrix())
                    .setProjectionMatrix(_projection)
                    .draw(_sphereMesh);
        }
    }

    // Undo Magnum's changes to the graphics state
    Magnum::GL::Context::current().resetState(GL::Context::State::EnterExternal);;
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
