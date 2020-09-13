//
// Created by jackcamp on 9/12/20.
//

#include "Interactive.h"
#include "../Controller/Application.h"

View::Interactive::Interactive(Controller::Application &application) {

    auto builder = Gtk::Builder::create();
    builder->add_from_file("../../ui/interactive.glade");

    Gtk::Window *window = nullptr;
    builder->get_widget<Gtk::Window>("window", window);
    assert(window);

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
