//
// Created by jackcamp on 12/21/20.
//

#include "View.h"

#include <utility>

View::View::View() : Gtk::GLArea() {

    set_size_request(400, 400);

    set_auto_render();

    set_required_version(4, 5);

    signal_realize().connect(sigc::mem_fun(this, &View::setupGL));
    signal_render().connect(sigc::mem_fun(this, &View::render));
    signal_resize().connect(sigc::mem_fun(this, &View::resize));
}

void View::View::setDrawables(const std::deque<Drawable::Drawable> *drawables) {
    _drawables = drawables;

    if (!_drawables) return;

    _sphereInstances = Containers::Array<SphereInstanceData>{Containers::NoInit, _drawables->size()};
    auto sphereInstance = _sphereInstances.begin();
    for (auto &drawable : *_drawables) {

        sphereInstance->transformation =
                Matrix4::translation({drawable._position->x, drawable._position->y, drawable._position->z})
                * Matrix4::scaling(Vector3{drawable._radius});

        sphereInstance->normal = sphereInstance->transformation.normalMatrix();

        sphereInstance->color = drawable._color;

        sphereInstance++;
    }

    _sphereMesh.setInstanceCount(_sphereInstances.size());

    queue_render();
}

void View::View::draw(GL::Framebuffer &framebuffer) {
    framebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
}

void View::View::setupGL() {
    Widget::realize();

    make_current();
    _context.create();

    set_has_depth_buffer();
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
}

bool View::View::render(const Glib::RefPtr<Gdk::GLContext> &context) {

    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    GLint framebufferID;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);
    auto framebuffer =
            GL::Framebuffer::wrap(framebufferID, {{}, {get_width(), get_height()}});

    draw(framebuffer);

    GL::Context::current().resetState(GL::Context::State::EnterExternal);

    return false;
}

void View::View::resize(int width, int height) {}
