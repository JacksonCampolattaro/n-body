//
// Created by jackcamp on 9/1/20.
//

#include "InteractiveView.h"

#include "../Controller/Application.h"

View::InteractiveView::InteractiveView(Controller::Application &application,
                                       std::shared_ptr<Model::Simulation> simulation) :
        _simulation(*simulation),
        _window(),
        _vbox(Gtk::Orientation::ORIENTATION_VERTICAL),
        _hbox(Gtk::Orientation::ORIENTATION_HORIZONTAL),
        _viewport(),
        _button_advance(">"),
        _button_run(">>>") {

    _window.set_default_size(1000, 1000);
    _window.add(_vbox);

    _button_run.signal_clicked().connect(sigc::mem_fun(this, &InteractiveView::on_run_clicked));

    _vbox.pack_start(_viewport);

    _vbox.pack_start(_progressBar, false, true);

    _vbox.pack_start(_hbox, false, true);

    _hbox.set_homogeneous();
    _hbox.pack_start(_button_advance);
    _hbox.pack_start(_button_run);

    _window.show();
    _vbox.show();
    _viewport.show();
    _progressBar.show();
    _hbox.show();
    _button_advance.show();
    _button_run.show();

    application.add_window(_window);

    _button_advance.signal_clicked().connect(sigc::mem_fun(this, &InteractiveView::on_advance_clicked));

    simulation->signal_drawables_changed.connect(sigc::mem_fun(&_viewport, &SimulationViewport::draw));
    simulation->signal_update_progress.connect(sigc::mem_fun(this, &InteractiveView::on_update_progress));
    simulation->signal_drawables_changed.emit(simulation->_drawables);
}

void View::InteractiveView::on_advance_clicked() {

    _viewport.signal_render_complete.clear();
    _workerThread = new std::thread([this] {
       _simulation.update();
    });
}

void View::InteractiveView::on_run_clicked() {

    _viewport.signal_render_complete.connect(sigc::mem_fun(&_simulation, &Model::Simulation::update));
    _simulation.update();
}

void View::InteractiveView::on_update_progress(float progress, const std::string &description) {

    _progressBar.set_fraction(progress);
}
