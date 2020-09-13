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
        virtual void on_start_simulation() {

            if (_worker.joinable())
                _worker.join();

            _worker = std::thread([this] {
                signal_start_simulation.emit();
            });
        }

        virtual void on_simulation_progress(float progress, const std::string &status) {

            //_progress = std::max(progress, _progress);
            _progress = progress;
            _status = status;

            spdlog::trace("Progress updated to: {}", _progress);

            _progress_dispatcher.emit();
        }

        virtual void on_drawables_changed(const std::deque<Model::Drawable::Drawable> &drawables) = 0;

        virtual void on_simulation_complete() = 0;

    protected:

        std::thread _worker;

        Glib::Dispatcher _progress_dispatcher;
        float _progress = 0.0;
        std::string _status = "not started";
    };

}


#endif //N_BODY_VIEW_H
