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

namespace View {

    class SimulationViewport : public Gtk::GLArea {

    public:

        SimulationViewport();

        void attach_simulation(std::shared_ptr<Model::Simulation> simulation);

    protected:

        void onRealize();
        bool onRender(const Glib::RefPtr<Gdk::GLContext>& context);
        void onResize(int width, int height);
        void onUnrealize();

        Magnum::Platform::GLContext _context;
        std::shared_ptr<Model::Simulation> _simulation;
    };

}

#endif //N_BODY_SIMULATIONVIEWPORT_H
