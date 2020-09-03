//
// Created by jackcamp on 9/1/20.
//

#ifndef N_BODY_SIMULATIONVIEWPORT_H
#define N_BODY_SIMULATIONVIEWPORT_H


#include "../Controller/Logger.h"
#include "../Model/Simulation.h"

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

    class SimulationViewport : public Gtk::GLArea {

    public:

        SimulationViewport();

        void attach_drawables(std::shared_ptr<std::vector<Model::Drawable::Drawable>> drawables);

        sigc::signal<void> signal_render_complete;

    protected:

        void onRealize();
        bool onRender(const Glib::RefPtr<Gdk::GLContext>& context);
        void onResize(int width, int height);
        void onUnrealize();

        Platform::GLContext _context{NoCreate};

        std::shared_ptr<std::vector<Model::Drawable::Drawable>> _drawables;

    private:

        GL::Mesh _sphereMesh{NoCreate};
        Shaders::Phong _shader{NoCreate};
        Matrix4 _projection;

        float _aspectRatio;
    };

}

#endif //N_BODY_SIMULATIONVIEWPORT_H
