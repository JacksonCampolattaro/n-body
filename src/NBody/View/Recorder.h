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
#include <Magnum/DebugTools/Screenshot.h>

#include <gdkmm/display.h>

namespace NBody {

    using namespace Magnum;

    class Recorder {
    private:

        const Simulation &_simulation;
        Camera &_camera;

    public:

        Recorder(Camera &camera, const Simulation &simulation);

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
