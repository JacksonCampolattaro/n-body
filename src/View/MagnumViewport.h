//
// Created by jackcamp on 2/4/20.
//

#ifndef N_BODY_MAGNUMVIEWPORT_H
#define N_BODY_MAGNUMVIEWPORT_H

#include "../Controller/Logger.h"

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>

namespace View {

    class MagnumViewport : public Gtk::GLArea {

    public:

        MagnumViewport();
        ~MagnumViewport() override;

    protected:

        void onRealize();
        bool onRender(const Glib::RefPtr<Gdk::GLContext>& context);
        void onResize(int width, int height);
        void onUnrealize();

        Magnum::Platform::GLContext _context;

    };

}


#endif //N_BODY_MAGNUMVIEWPORT_H
