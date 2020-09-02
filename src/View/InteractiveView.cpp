//
// Created by jackcamp on 9/1/20.
//

#include "InteractiveView.h"

#include "../Controller/Application.h"

View::InteractiveView::InteractiveView(Controller::Application &application, std::shared_ptr<Model::Simulation> simulation) :
        _window(),
        _vbox(Gtk::Orientation::ORIENTATION_VERTICAL),
        _hbox(Gtk::Orientation::ORIENTATION_HORIZONTAL),
        _viewport(),
        _button_advance(">"),
        _button_run(">>>") {

    _window.add(_vbox);

    _viewport.attach_simulation(simulation);
    _vbox.pack_start(_viewport);

    _vbox.pack_start(_hbox, false, true);

    _hbox.set_homogeneous();
    _hbox.pack_start(_button_advance);
    _hbox.pack_start(_button_run);

    _window.show();
    _vbox.show();
    _viewport.show();
    _hbox.show();
    _button_advance.show();
    _button_run.show();

    application.add_window(_window);
}
