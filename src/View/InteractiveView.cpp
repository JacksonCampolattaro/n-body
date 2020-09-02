//
// Created by jackcamp on 9/1/20.
//

#include "InteractiveView.h"

#include "../Controller/Application.h"

View::InteractiveView::InteractiveView() :
        _window(),
        _box(Gtk::Orientation::ORIENTATION_VERTICAL),
        _view(),
        _button("text") {

}

void View::InteractiveView::attach_application(Controller::Application *application) {

    // Create a window
    spdlog::debug("Creating a window with a viewport");

    _window.add(_box);

    _button.set_margin_top(5);
    _button.set_margin_bottom(5);
    _button.set_margin_left(5);
    _button.set_margin_right(5);

    _box.pack_start(_view);
    _box.pack_start(_button, false, true);

    // Attach the window to this application
    spdlog::debug("Adding window to application");
    application->add_window(_window);
    _window.show();
    _box.show();
    _view.show();
    _button.show();

    // Run the program itself
    spdlog::debug("Running the program");
}
