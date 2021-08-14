//
// Created by jackcamp on 7/15/21.
//

#include "Simple.h"

Interface::Simple::Simple(Simulation &simulation) : Interface(simulation), _view(simulation) {
    add(_view);
    _view.show();
}
