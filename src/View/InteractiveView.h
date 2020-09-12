//
// Created by jackcamp on 9/1/20.
//

#ifndef N_BODY_INTERACTIVEVIEW_H
#define N_BODY_INTERACTIVEVIEW_H

#include "View.h"

#include "SimulationViewport.h"

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <thread>

namespace View {

    class InteractiveView : public View {
    public:
        explicit InteractiveView(Controller::Application &application, std::shared_ptr<Model::Simulation> simulation);

        void on_advance_clicked();
        void on_run_clicked();

    private:

        Model::Simulation &_simulation;
        std::thread *_workerThread;

        Gtk::Window _window;
        Gtk::Box _vbox, _hbox;
        SimulationViewport _viewport;
        Gtk::Button _button_advance, _button_run;
    };

}


#endif //N_BODY_INTERACTIVEVIEW_H
