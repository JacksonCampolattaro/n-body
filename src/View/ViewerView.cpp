//
// Created by jackcamp on 9/1/20.
//

#include "ViewerView.h"

#include "../Controller/Application.h"

View::ViewerView::ViewerView() {}

void View::ViewerView::attach_application(Controller::Application *application) {


    spdlog::debug("Creating a window with a viewport");

    _window.add(_view);


    spdlog::debug("Adding window to application");
    application->add_window(_window);
    _window.show();
    _view.show();

    spdlog::debug("Running the program");
}
