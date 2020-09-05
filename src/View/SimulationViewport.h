//
// Created by jackcamp on 9/1/20.
//

#ifndef N_BODY_SIMULATIONVIEWPORT_H
#define N_BODY_SIMULATIONVIEWPORT_H


#include "../Controller/Logger.h"
#include "../Model/Drawable/Drawable.h"

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

using namespace Magnum;

namespace View {

    struct SphereInstanceData {
        Matrix4 transformation;
        Matrix3x3 normal;
        Color3 color;
    };

    class SimulationViewport : public Gtk::GLArea {

    public:

        SimulationViewport();

        void draw(const std::deque<Model::Drawable::Drawable> &drawables);

        sigc::signal<void> signal_render_complete;

    protected:

        void onRealize();

        bool onRender(const Glib::RefPtr<Gdk::GLContext> &context);

        void onResize(int width, int height);

        void onUnrealize();

        Platform::GLContext _context{NoCreate};

        GL::Mesh _sphereMesh{NoCreate};
        GL::Buffer _sphereInstanceBuffer{NoCreate};
        Containers::Array<SphereInstanceData> _sphereInstanceData;
        Shaders::Phong _shader{NoCreate};
        Matrix4 _projection;
        float _aspectRatio;
    };

}

#endif //N_BODY_SIMULATIONVIEWPORT_H
