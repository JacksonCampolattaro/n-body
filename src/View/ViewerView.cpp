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

    _viewport.attach_drawables(
            std::make_shared<std::vector<Model::Drawable::Drawable>>(simulation->_drawables));

    simulation->signal_update_complete.connect(sigc::mem_fun(&_viewport, &SimulationViewport::queue_render));
    _viewport.signal_render_complete.connect(sigc::mem_fun(simulation.get(), &Model::Simulation::update));

    _window.show();
    _viewport.show();

    application.add_window(_window);
}
