//
// Created by jackcamp on 2/16/22.
//

#include "Interactive.h"

#include <gtkmm/binlayout.h>

//UI::Interactive::Interactive(NBody::Simulation &simulation) :
//        Gtk::Paned(Gtk::Orientation::HORIZONTAL),
//        _simulation{simulation},
//        _camera{
//                Vector3::zAxis(-20.0f),
//                {}, Vector3::yAxis(),
//                45.0_degf,
//                Vector2i{400, 400}
//        },
//        _view{_camera, _simulation},
//        _sidebar(_camera, _simulation) {
//
//    set_expand();
//
//    set_start_child(_view);
//    set_end_child(_sidebar);
//}

UI::Interactive::Interactive(Gtk::ApplicationWindow::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder,
                             NBody::Simulation &simulation) :
        Gtk::ApplicationWindow(cobject),
        _simulation{simulation},
        _camera{
                Vector3::zAxis(-20.0f),
                {}, Vector3::yAxis(),
                45.0_degf,
                Vector2i{400, 400}
        },
        _view{_camera, _simulation},
        _sidebar(_camera, _simulation) {

    auto &flap = *builder->get_widget<Gtk::Box>("FlapBox");
    auto &content = *builder->get_widget<Gtk::Box>("ContentBox");

    // TODO this should be done automatically
    flap.set_size_request(500, -1);

    _view.set_hexpand();

    flap.append(_sidebar);
    content.append(_view);
}
