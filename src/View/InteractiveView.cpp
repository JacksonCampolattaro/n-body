//
// Created by jackcamp on 9/1/20.
//

#include "InteractiveView.h"

#include "../Controller/Application.h"

View::InteractiveView::InteractiveView(Controller::Application &application,
                                       std::shared_ptr<Model::Simulation> simulation) :
        _window(),
        _vbox(Gtk::Orientation::ORIENTATION_VERTICAL),
        _hbox(Gtk::Orientation::ORIENTATION_HORIZONTAL),
        _viewport(),
        _button_advance(">"),
        _button_run(">>>") {

    _window.set_default_size(1000, 1000);
    _window.add(_vbox);

    _viewport.attach_drawables(
            std::make_shared<std::vector<Model::Drawable::Drawable>>(simulation->_drawables));

    _button_advance.signal_clicked().connect(sigc::mem_fun(simulation.get(), &Model::Simulation::update));
    simulation->signal_update_complete.connect(sigc::mem_fun(&_viewport, &SimulationViewport::draw));
    _viewport.draw(simulation->_drawables);

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
