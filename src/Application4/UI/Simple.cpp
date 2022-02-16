#include "Simple.h"

UI::Simple::Simple(NBody::Simulation &simulation) :
        Gtk::ApplicationWindow(),
        _simulation{simulation},
        _camera{
                Vector3::zAxis(-10.0f),
                {0, 0, 0}, Vector3::yAxis(),
                45.0_degf,
                Vector2i{400, 400}
        },
        _view{_camera, simulation} {

    set_child(_view);
}
