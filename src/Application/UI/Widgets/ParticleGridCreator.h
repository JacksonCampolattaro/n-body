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

    class ParticleGridCreator : public BuilderWidget<Gtk::Box> {
    private:

        NBody::Simulation &_simulation;

        CompactPositionEntry &_gridCornerEntry;
        SimplePositiveSizeEntry &_gridXSizeEntry;
        SimplePositiveSizeEntry &_gridYSizeEntry;
        SimplePositiveSizeEntry &_gridZSizeEntry;
        FloatEntry &_spacingEntry;

        CompactVelocityEntry &_velocityEntry;
        FloatEntry &_massEntry;
        Gtk::CheckButton &_accelerationEntry;

        Gtk::ColorButton &_colorEntry;
        FloatEntry &_radiusEntry;

        void createGrid();

    public:

        ParticleGridCreator(Gtk::Box::BaseObjectType *cobject,
                            const Glib::RefPtr<Gtk::Builder> &builder,
                            NBody::Simulation &simulation);

    public:

        sigc::signal<void()> signal_done;
    };

}


#endif //N_BODY_PARTICLEGRIDCREATOR_H
