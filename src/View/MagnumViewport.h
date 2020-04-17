//
// Created by jackcamp on 2/4/20.
//

#ifndef N_BODY_MAGNUMVIEWPORT_H
#define N_BODY_MAGNUMVIEWPORT_H

#include "../Controller/Logger.h"

#include <gtkmm/glarea.h>

namespace View {

    class MagnumViewport : public Gtk::GLArea {

    public:

        MagnumViewport();
        ~MagnumViewport() override;

    };

}


#endif //N_BODY_MAGNUMVIEWPORT_H
