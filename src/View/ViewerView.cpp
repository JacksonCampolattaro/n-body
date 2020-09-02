//
// Created by jackcamp on 9/1/20.
//

#include "ViewerView.h"

#include "../Controller/Application.h"

View::ViewerView::ViewerView(Controller::Application &application, std::shared_ptr<Model::Simulation> simulation) :
        _window(),
        _viewport() {

    _window.add(_viewport);

    _window.show();
    _viewport.show();

    application.add_window(_window);

    _viewport.attach_simulation(simulation);
}
