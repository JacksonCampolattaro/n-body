//
// Created by jackcamp on 2/16/22.
//

#include "Interactive.h"

#include <gtkmm/binlayout.h>

UI::Interactive::Interactive(Gtk::ApplicationWindow::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder,
                             NBody::Simulation &simulation, NBody::Physics::Rule &rule, NBody::MultiSolver &solver) :
        BuilderWidget<Gtk::ApplicationWindow>(cobject, builder),
        _simulation{simulation},
        _camera{
                Vector3::zAxis(-20.0f),
                Vector3::yAxis(),
                45.0_degf,
        },
        _view{_camera, _simulation},
        _sidebar(_camera, _simulation, rule, solver) {

    auto &flap = getWidget<Gtk::Box>("FlapBox");
    auto &content = getWidget<Gtk::Box>("ContentBox");

    _view.set_hexpand();

    flap.append(_sidebar);
    content.append(_view);

    // Make sure the sidebar doesn't get squeezed too tight
    Gtk::Requisition min, natural;
    flap.get_preferred_size(min, natural);
    flap.set_size_request(natural.get_width(), natural.get_height());

    _view.signal_doneRendering().connect([&]() {
        spdlog::trace("done rendering");
    });

    _view.set_cursor(Gdk::Cursor::create("crosshair"));
}
