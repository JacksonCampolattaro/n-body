//
// Created by jackcamp on 12/21/20.
//

#include "View.h"

#include <utility>

NBody::View::View::View() : Gtk::GLArea() {

    set_size_request(400, 400);

    set_auto_render();

    set_required_version(4, 5);

    signal_realize().connect(sigc::mem_fun(this, &View::setupGL));
    signal_render().connect(sigc::mem_fun(this, &View::render));
    signal_resize().connect(sigc::mem_fun(this, &View::resize));
}

void NBody::View::View::setDrawables(const Simulation::Point_set &points) {
    typedef Simulation::Point_set ::Property_map<CGAL::Color> Color_map;

    if (points.empty()) return;

    auto colors = points.property_map<CGAL::Color>("color").first;

    _sphereInstances = Containers::Array<SphereInstanceData>{Containers::NoInit, points.size()};
    auto sphereInstance = _sphereInstances.begin();
    for (auto &index : points) {

        auto color = colors[index];

        sphereInstance++;
    }

//    _drawables = drawables;
//
//    if (!_drawables) return;
//
//    _sphereInstances = Containers::Array<SphereInstanceData>{Containers::NoInit, _drawables->size()};
//    auto sphereInstance = _sphereInstances.begin();
//    for (
//        auto &drawable
//            : *_drawables) {
//
//        sphereInstance->
//                transformation =
//                Matrix4::translation({drawable._position->x, drawable._position->y, drawable._position->z})
//                * Matrix4::scaling(Vector3{drawable._radius});
//
//        sphereInstance->
//                normal = sphereInstance->transformation.normalMatrix();
//
//        sphereInstance->
//                color = drawable._color;
//
//        sphereInstance++;
//    }
//

    _sphereMesh.setInstanceCount(_sphereInstances.size());

    queue_render();

}

void NBody::View::View::draw(GL::Framebuffer &framebuffer) {
    framebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    _projection =
            Matrix4::perspectiveProjection(
                    35.0_degf, _aspectRatio, 0.01f, 100000.0f) *
            Matrix4::translation(Vector3::zAxis(-35.0f));

    _sphereInstanceBuffer.setData(_sphereInstances, GL::BufferUsage::DynamicDraw);
    _shader.setProjectionMatrix(_projection)
            .draw(_sphereMesh);
}

void NBody::View::View::setupGL() {
    Widget::realize();

    make_current();
    _context.create();

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

bool NBody::View::View::render(const Glib::RefPtr<Gdk::GLContext> &context) {

    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    GLint framebufferID;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);
    auto framebuffer =
            GL::Framebuffer::wrap(framebufferID, {{},
                                                  {get_width(), get_height()}});

    draw(framebuffer);

    GL::Context::current().resetState(GL::Context::State::EnterExternal);

    return false;
}

void NBody::View::View::resize(int width, int height) {

    _aspectRatio = (float) width / (float) height;
}
