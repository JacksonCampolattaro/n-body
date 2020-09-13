//
// Created by jackcamp on 9/1/20.
//

#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include "../Model/Simulation.h"
#include "../Model/Drawable/Drawable.h"

#include <spdlog/spdlog.h>
#include <giomm.h>
#include <thread>

namespace Controller {
    class Application;
}

namespace View {

    class View {
    public:

        // Signals
        sigc::signal<void()> signal_start_simulation;

        // Sinks
        virtual void on_start_simulation() = 0;
        virtual void on_simulation_progress(float progress, const std::string &status) = 0;
        virtual void on_drawables_changed(const std::deque<Model::Drawable::Drawable> &drawables) = 0;
        virtual void on_simulation_complete() = 0;

    protected:

        Glib::Dispatcher _dispatcher;
        std::thread _worker;

    };

}


#endif //N_BODY_VIEW_H
