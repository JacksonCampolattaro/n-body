//
// Created by jackcamp on 7/17/21.
//

#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include <optional>

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/Math/Color.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>

#include <NBody/Simulation/Simulation.h>

#include "ArcBallCamera.h"

using namespace Magnum;
using namespace Math::Literals;

using Object3D = SceneGraph::Object<SceneGraph::MatrixTransformation3D>;
using Scene3D = SceneGraph::Scene<SceneGraph::MatrixTransformation3D>;

namespace NBody {

    class View : public Gtk::GLArea {
    protected:

        const Simulation &_simulation;

        Platform::GLContext _context{NoCreate, 0, nullptr};

        ArcBallCamera &_camera;

    public:

        View(ArcBallCamera &camera, const Simulation &simulation);

        void onRealize();

        void onUnrealize() {}

        bool onRender(const Glib::RefPtr<Gdk::GLContext> &);

        void onResize(int width, int height);

    public:
        // Slots

        sigc::slot<void()> slot_renderNeeded;

    };
}


#endif //N_BODY_VIEW_H
