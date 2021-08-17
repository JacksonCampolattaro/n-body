//
// Created by jackcamp on 7/15/21.
//

#include "Simple.h"

Interface::Simple::Simple(Simulation &simulation) :
        Interface(simulation),
        _camera{
                Vector3::zAxis(-10.0f),
                {}, Vector3::yAxis(),
                45.0_degf,
                Vector2i{400, 400},
        },
        _view(_camera, simulation) {

    add(_view);
    _view.show();
}
