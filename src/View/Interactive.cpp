//
// Created by jackcamp on 9/12/20.
//

#include "Interactive.h"
#include "../Controller/Application.h"

View::Interactive::Interactive(Controller::Application &application) {

    auto builder = Gtk::Builder::create();
    builder->add_from_file("../../ui/interactive.glade");

    Gtk::Window *window = nullptr;
    builder->get_widget("window", window);
    assert(window);

    Gtk::GLArea *viewport = nullptr;
    builder->get_widget("viewport", viewport);
    assert(viewport);

    Gtk::Button *stopButton = nullptr;
    builder->get_widget("stopButton", stopButton);
    assert(stopButton);

    Gtk::Button *updateButton = nullptr;
    builder->get_widget("updateButton", updateButton);
    assert(updateButton);

    Gtk::Button *runButton = nullptr;
    builder->get_widget("runButton", runButton);
    assert(runButton);

    Gtk::ProgressBar *progressBar = nullptr;
    builder->get_widget("progressBar", progressBar);
    assert(progressBar);

    application.add_window(*window);
    window->show();

}

void View::Interactive::on_start_simulation() {

}

void View::Interactive::on_simulation_progress(float progress, const std::string &status) {

}

void View::Interactive::on_drawables_changed(const std::deque<Model::Drawable::Drawable> &drawables) {

}

void View::Interactive::on_simulation_complete() {

}
