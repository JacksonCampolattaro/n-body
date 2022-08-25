//
// Created by Jackson Campolattaro on 8/24/22.
//

#ifndef N_BODY_PARTICLEGRIDCREATOR_H
#define N_BODY_PARTICLEGRIDCREATOR_H

#include <gtkmm/colorbutton.h>

#include <NBody/Simulation/Simulation.h>
#include <gtkmm/checkbutton.h>

#include "Application/UI/Widgets/Entry/VectorEntry.h"
#include "Application/UI/Widgets/Entry/IntEntry.h"

#include "BuilderWidget.h"

namespace UI {

    class ParticleGridCreator : public BuilderWidget {
    private:

        NBody::Simulation &_simulation;

        CompactPositionEntry &_gridCornerEntry;
        SimplePositiveSizeEntry &_gridXSizeEntry;
        SimplePositiveSizeEntry &_gridYSizeEntry;
        SimplePositiveSizeEntry &_gridZSizeEntry;
        FloatEntry &_spacingEntry;

        FloatEntry &_massEntry;
        CompactVelocityEntry &_velocityEntry;
        Gtk::CheckButton &_activeEntry;
        Gtk::CheckButton &_passiveEntry;

        Gtk::ColorButton &_colorEntry;
        FloatEntry &_radiusEntry;

        void createGrid();

    public:

        explicit ParticleGridCreator(NBody::Simulation &simulation);

    public:

        sigc::signal<void()> signal_done;
    };

}


#endif //N_BODY_PARTICLEGRIDCREATOR_H
