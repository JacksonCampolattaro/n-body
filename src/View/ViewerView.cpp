//
// Created by jackcamp on 9/1/20.
//

#include "ViewerView.h"

#include "../Controller/Application.h"

View::ViewerView::ViewerView(Controller::Application &application, std::shared_ptr<Model::Simulation> simulation) :
        _window(),
        _viewport() {

    _viewport.attach_drawables(
            std::make_shared<std::vector<Model::Drawable::Drawable>>(simulation->_drawables));
    _window.add(_viewport);

    _window.show();
    _viewport.show();

    application.add_window(_window);
}
