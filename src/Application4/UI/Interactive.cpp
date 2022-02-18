//
// Created by jackcamp on 2/16/22.
//

#include "Interactive.h"

UI::Interactive::Interactive(NBody::Simulation &simulation) :
        Gtk::ApplicationWindow(),
        _simulation{simulation},
        _camera{
                Vector3::zAxis(-20.0f),
                {}, Vector3::yAxis(),
                45.0_degf,
                Vector2i{400, 400}
        },
        _paned(Gtk::Orientation::HORIZONTAL),
        _view{_camera, _simulation},
        _sidebar(_camera, _simulation) {

    set_child(_paned);

    _paned.set_start_child(_view);
    _paned.set_end_child(_sidebar);
}
