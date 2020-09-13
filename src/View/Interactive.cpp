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
    _painter = std::make_unique<Painter>(*viewport);

    Gtk::Button *stopButton = nullptr;
    builder->get_widget("stopButton", stopButton);
    assert(stopButton);

    Gtk::Button *updateButton = nullptr;
    builder->get_widget("updateButton", updateButton);
    assert(updateButton);
    updateButton->signal_clicked().connect(sigc::mem_fun(*this, &Interactive::on_start_simulation));

    Gtk::Button *runButton = nullptr;
    builder->get_widget("runButton", runButton);
    assert(runButton);
    // TODO: Connect signals for looping
    runButton->signal_clicked().connect(sigc::mem_fun(*this, &Interactive::on_start_simulation));

    Gtk::ProgressBar *progressBar = nullptr;
    builder->get_widget("progressBar", progressBar);
    assert(progressBar);
    _progress_dispatcher.connect([progressBar, this]{
        progressBar->set_fraction(_progress);
        progressBar->set_text(_status);
    });
    _progress_dispatcher.emit();

    application.add_window(*window);
    window->show();

}

void View::Interactive::on_drawables_changed(const std::deque<Model::Drawable::Drawable> &drawables) {

    _painter->draw(drawables);
}

void View::Interactive::on_simulation_complete() {

}
