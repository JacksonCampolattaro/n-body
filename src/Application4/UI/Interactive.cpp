//
// Created by jackcamp on 2/16/22.
//

#include "Interactive.h"

#include <gtkmm/binlayout.h>

UI::Interactive::Interactive(NBody::Simulation &simulation) :
        Gtk::Paned(Gtk::Orientation::HORIZONTAL),
        _simulation{simulation},
        _camera{
                Vector3::zAxis(-20.0f),
                {}, Vector3::yAxis(),
                45.0_degf,
                Vector2i{400, 400}
        },
        _view{_camera, _simulation},
        _sidebar(_camera, _simulation) {

    set_expand();

    set_start_child(_view);
    set_end_child(_sidebar);
}
