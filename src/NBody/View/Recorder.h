//
// Created by Jackson Campolattaro on 8/26/22.
//

#ifndef N_BODY_RECORDER_H
#define N_BODY_RECORDER_H

#include "Camera.h"

#include <NBody/Simulation/Simulation.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/GL/Context.h>
#include <Magnum/Image.h>

#include <gdkmm/display.h>
#include <gdkmm/pixbuf.h>
#include <gdkmm/glcontext.h>

namespace NBody {

    using namespace Magnum;

    Glib::RefPtr<Gdk::Pixbuf> toPixbuf(const Image2D &texture);

    class Recorder {
    private:

        const Simulation &_simulation;
        Camera &_camera;
        sigc::signal<void()> &_trigger;

        Glib::RefPtr<Gdk::GLContext> _context;

        std::vector<Image2D> _video;

    public:

        Recorder(Camera &camera, const Simulation &simulation, sigc::signal<void()> &trigger);

        Image2D snapshot(const Vector2i &resolution);

        void startVideo(const Vector2i &resolution);

        void encodeVideo();

        void takeImage(const Vector2i &resolution);

//        void startVideo();
//
//        void stopVideo();
//
//        void resetVideo();
//
//    private:
//
//        void takeFrame();
    };

}


#endif //N_BODY_RECORDER_H
