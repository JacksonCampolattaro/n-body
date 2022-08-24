//
// Created by jackcamp on 2/20/22.
//

#ifndef N_BODY_PARTICLEENTRY_H
#define N_BODY_PARTICLEENTRY_H

#include "NBody/Simulation/Simulation.h"

#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/builder.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/checkbutton.h>

#include "Application/UI/Widgets/View/VectorView.h"
#include "VectorEntry.h"

#include "ParticlePositionEntry.h"
#include "ParticleVelocityEntry.h"
#include "ParticleMassEntry.h"
#include "ParticleColorEntry.h"
#include "ParticleRadiusEntry.h"
#include "ParticleTagEntry.h"

namespace UI {

    class ParticleEntry : public Gtk::Box {
    private:

        Glib::RefPtr<Gtk::Builder> _builder;

        ParticlePositionEntry &_positionEntry;
        ParticleVelocityEntry &_velocityEntry;

        ParticleMassEntry &_massEntry;
        ParticleActiveTagEntry &_activeEntry;
        ParticlePassiveTagEntry &_passiveEntry;

        ParticleColorEntry &_colorEntry;
        ParticleRadiusEntry &_radiusEntry;

    public:

        ParticleEntry();

        void bind(std::shared_ptr<NBody::Simulation::Particle> &particle);

    };

}

#endif //N_BODY_PARTICLEENTRY_H
