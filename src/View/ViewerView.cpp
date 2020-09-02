//
// Created by jackcamp on 9/1/20.
//

#include "ViewerView.h"

#include "../Controller/Application.h"

View::ViewerView::ViewerView() :
        _window(),
        _view() {

    _window.add(_view);

    _window.show();
    _view.show();
}

void View::ViewerView::attach_application(Controller::Application *application) {
    application->add_window(_window);
}
