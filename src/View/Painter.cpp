//
// Created by jackcamp on 9/12/20.
//

#include "Painter.h"

using namespace Magnum;
using namespace Math::Literals;

View::Painter::Painter(Gtk::GLArea &glArea) : _glArea(glArea) {

    // Automatically re-render everything each time it needs to be redrawn
    glArea.set_auto_render();

    // Set desired OpenGL version
    glArea.set_required_version(4, 5);

    // Connecting signals to their respective handlers
    glArea.signal_realize().connect(sigc::mem_fun(this, &Painter::on_realize));
    glArea.signal_render().connect(sigc::mem_fun(this, &Painter::on_render));
    glArea.signal_resize().connect(sigc::mem_fun(this, &Painter::on_resize));
    glArea.signal_unrealize().connect(sigc::mem_fun(this, &Painter::on_unrealize));

}

void View::Painter::draw(const std::deque<Model::Drawable::Drawable> &drawables) {

    _sphereInstanceData = Containers::Array<SphereInstanceData>{Containers::NoInit, drawables.size()};
    for (int i = 0; i < drawables.size(); ++i) {

        auto position = drawables[i].position();
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

    _glArea.queue_render();
}

void View::Painter::on_realize() {

    // Make the OpenGL context current, and then configure it
    _glArea.make_current();
    _context.create();

    // Configure the graphics features
    _glArea.set_has_depth_buffer();
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

bool View::Painter::on_render(const Glib::RefPtr<Gdk::GLContext> &context) {

    // Reset state, so Gtkmm's defaults won't interfere with Magnum
    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    // Get the ID of the current framebuffer
    GLint framebufferID;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);

    // Attach Magnum's framebuffer manager to the framebuffer provided by Gtkmm
    auto framebuffer =
            GL::Framebuffer::wrap(framebufferID, {{},
                                                  {_glArea.get_width(), _glArea.get_height()}});

    // Clear
    framebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    _projection =
            Matrix4::perspectiveProjection(
                    35.0_degf, _aspectRatio, 0.01f, 100000.0f) *
            Matrix4::translation(Vector3::zAxis(-35.0f));

    _sphereInstanceBuffer.setData(_sphereInstanceData, GL::BufferUsage::DynamicDraw);
    _shader.setProjectionMatrix(_projection)
            .draw(_sphereMesh);

    // Undo Magnum's changes to the graphics state
    Magnum::GL::Context::current().resetState(GL::Context::State::EnterExternal);;

    signal_render_complete.emit();
    return false;
}

void View::Painter::on_resize(int width, int height) {

    _aspectRatio = (float) width / (float) height;
}

void View::Painter::on_unrealize() {

}
