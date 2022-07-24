//
// Created by jackcamp on 2/16/22.
//

#include "Interactive.h"

#include <gtkmm/binlayout.h>
#include <adwaita.h>

UI::Interactive::Interactive(Gtk::ApplicationWindow::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder,
                             NBody::Simulation &simulation, NBody::Physics::Rule &rule, NBody::MultiSolver &solver) :
        Gtk::ApplicationWindow(cobject),
        _simulation{simulation},
        _camera{
                simulation,
                Vector3::zAxis(-20.0f),
                {}, Vector3::yAxis(),
                45.0_degf,
                Vector2i{400, 400}
        },
        _view{_camera, _simulation},
        _sidebar(_camera, _simulation, rule, solver) {

    auto &flap = *builder->get_widget<Gtk::Box>("FlapBox");
    auto &content = *builder->get_widget<Gtk::Box>("ContentBox");

    _view.set_hexpand();
    solver.signal_finished().connect(_view.slot_renderNeeded);

    flap.append(_sidebar);
    content.append(_view);

    // Make sure the sidebar doesn't get squeezed too tight
    Gtk::Requisition min, natural;
    flap.get_preferred_size(min, natural);
    flap.set_size_request(natural.get_width(), natural.get_height());

    _view.signal_doneRendering.connect([&]() {
        spdlog::trace("done rendering");
    });

    if (adw_style_manager_get_dark(adw_style_manager_get_default())) {
        _camera.setBackgroundColor({0.12, 0.12, 0.1, 1.0});
    } else {
        _camera.setBackgroundColor({0.9, 0.9, 0.92, 1.0});
    }

    _view.set_cursor(Gdk::Cursor::create("crosshair"));

}
