//
// Created by jackcamp on 9/1/20.
//

#include "ViewerView.h"

#include "../Controller/Application.h"

View::ViewerView::ViewerView(Controller::Application &application, std::shared_ptr<Model::Simulation> simulation) :
        _window(),
        _viewport() {

    _window.set_default_size(1000, 1000);
    _window.add(_viewport);

    simulation->signal_drawables_changed.connect(sigc::mem_fun(&_viewport, &SimulationViewport::draw));
    _viewport.signal_render_complete.connect(sigc::mem_fun(simulation.get(), &Model::Simulation::update));

    _window.show();
    _viewport.show();

    application.add_window(_window);
}
