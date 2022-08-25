//
// Created by Jackson Campolattaro on 8/24/22.
//

#ifndef N_BODY_PARTICLEGRIDCREATOR_H
#define N_BODY_PARTICLEGRIDCREATOR_H

#include <gtkmm/colorbutton.h>

#include <NBody/Simulation/Simulation.h>
#include <gtkmm/checkbutton.h>

#include "Application/UI/Widgets/Entry/VectorEntry.h"

#include "BuilderWidget.h"

namespace UI {

    class ParticleGridCreator : public BuilderWidget {
    private:

        CompactPositionEntry &_gridCornerEntry;
        // todo: x, y, and z "nonzero int" entries
        FloatEntry &_spacingEntry;

        FloatEntry &_massEntry;
        CompactVelocityEntry &_velocityEntry;
        Gtk::CheckButton &_activeEntry;
        Gtk::CheckButton &_passiveEntry;

        Gtk::ColorButton &_colorEntry;
        FloatEntry &_radiusEntry;

    public:

        explicit ParticleGridCreator(NBody::Simulation &simulation);
    };

}


#endif //N_BODY_PARTICLEGRIDCREATOR_H
