//
// Created by jackcamp on 9/12/20.
//

#include "Interactive.h"
#include "../Controller/Application.h"

View::Interactive::Interactive(Controller::Application &application) : View() {

    auto builder = Gtk::Builder::create();
    builder->add_from_file("../../ui/interactive.glade");

    Gtk::Window *window = nullptr;
    builder->get_widget("window", window);
    assert(window);

    Gtk::GLArea *viewport = nullptr;
    builder->get_widget("viewport", viewport);
    assert(viewport);
    _painter = std::make_unique<Painter>(*viewport);

    Gtk::Button *updateButton = nullptr;
    builder->get_widget("updateButton", updateButton);
    assert(updateButton);
    updateButton->signal_clicked().connect(sigc::mem_fun(*this, &Interactive::on_update_clicked));

    Gtk::Button *runButton = nullptr;
    builder->get_widget("runButton", runButton);
    assert(runButton);
    runButton->signal_clicked().connect(sigc::mem_fun(*this, &Interactive::on_run_clicked));

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

void View::Interactive::on_update_clicked() {

    on_start_simulation();
}

void View::Interactive::on_run_clicked() {

    _complete_dispatcher.connect(sigc::mem_fun(*this, &Interactive::on_start_simulation));
    on_start_simulation();
}
