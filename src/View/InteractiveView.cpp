//
// Created by jackcamp on 9/1/20.
//

#include "InteractiveView.h"

#include "../Controller/Application.h"

View::InteractiveView::InteractiveView(Controller::Application &application) :
        _window(),
        _vbox(Gtk::Orientation::ORIENTATION_VERTICAL),
        _hbox(Gtk::Orientation::ORIENTATION_HORIZONTAL),
        _view(),
        _button_advance(">"),
        _button_run(">>>") {

    _window.add(_vbox);

    _vbox.pack_start(_view);

    _vbox.pack_start(_hbox, false, true);

    _hbox.set_homogeneous();
    _hbox.pack_start(_button_advance);
    _hbox.pack_start(_button_run);

    _window.show();
    _vbox.show();
    _view.show();
    _hbox.show();
    _button_advance.show();
    _button_run.show();

    application.add_window(_window);
}
