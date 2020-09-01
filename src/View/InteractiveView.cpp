//
// Created by jackcamp on 9/1/20.
//

#include "InteractiveView.h"

#include "../Controller/Application.h"

View::InteractiveView::InteractiveView() {}

void View::InteractiveView::attach_application(Controller::Application *application) {

    // Create a window
    spdlog::debug("Creating a window with a viewport");
    auto window = new ViewerWindow();

    // Attach the window to this application
    spdlog::debug("Adding window to application");
    application->add_window(*window);
    window->show();

    // Run the program itself
    spdlog::debug("Running the program");
}
