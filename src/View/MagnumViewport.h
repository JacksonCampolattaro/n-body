//
// Created by jackcamp on 2/4/20.
//

#ifndef N_BODY_MAGNUMVIEWPORT_H
#define N_BODY_MAGNUMVIEWPORT_H

#include "../Controller/Logger.h"

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/GL/Framebuffer.h>

// FIXME Temporary (for testing):
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>

namespace View {

    using namespace Magnum;

    class MagnumViewport : public Gtk::GLArea {

    public:

        MagnumViewport();
        ~MagnumViewport() override;

    protected:

        void onRealize();
        bool onRender(const Glib::RefPtr<Gdk::GLContext>& context);
        void onResize(int width, int height);
        void onUnrealize();

        Platform::GLContext _context;

    };

}


#endif //N_BODY_MAGNUMVIEWPORT_H
