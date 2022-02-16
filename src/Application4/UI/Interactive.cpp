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
        _view{_camera, simulation} {

    set_child(_view);
}
