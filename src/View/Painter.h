//
// Created by jackcamp on 9/12/20.
//

#ifndef N_BODY_PAINTER_H
#define N_BODY_PAINTER_H

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Mesh.h>

#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>

#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Icosphere.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Trade/MeshData.h>

#include "../Model/Drawable/Drawable.h"

namespace View {

    struct SphereInstanceData {
        Matrix4 transformation;
        Matrix3x3 normal;
        Color3 color;
    };

    class Painter {

    public:

        // Setup

        Painter(Gtk::GLArea &glArea);

        void draw(const std::deque<Model::Drawable::Drawable> &drawables);

        // Signals

        sigc::signal<void()> signal_render_complete;

        // Sinks

        void on_realize();

        bool on_render(const Glib::RefPtr<Gdk::GLContext> &context);

        void on_resize(int width, int height);

        void on_unrealize();

    private:

        Gtk::GLArea &_glArea;

        Platform::GLContext _context{NoCreate};

        GL::Mesh _sphereMesh{NoCreate};
        GL::Buffer _sphereInstanceBuffer{NoCreate};
        Containers::Array<SphereInstanceData> _sphereInstanceData;
        Shaders::Phong _shader{NoCreate};
        Matrix4 _projection;
        float _aspectRatio;

    };

}


#endif //N_BODY_PAINTER_H
