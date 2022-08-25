#include "Simple.h"

UI::Simple::Simple(NBody::Simulation &simulation) :
        Gtk::ApplicationWindow(),
        _simulation{simulation},
        _camera{
                Vector3::zAxis(-20.0f),
                {}, Vector3::yAxis(),
                45.0_degf,
        },
        _view{_camera, simulation} {

    set_child(_view);
}
