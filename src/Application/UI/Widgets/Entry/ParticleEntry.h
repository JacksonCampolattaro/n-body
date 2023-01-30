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

#include "ParticlePositionEntry.h"
#include "ParticleVelocityEntry.h"
#include "ParticleMassCheckButton.h"
#include "ParticleMassEntry.h"
#include "ParticleAccelerationCheckButton.h"
#include "ParticleAccelerationEntry.h"
#include "ParticleColorEntry.h"
#include "ParticleRadiusEntry.h"

//#include "ParticleTagEntry.h"

#include "../BuilderWidget.h"

namespace UI {

    class ParticleEntry : public BuilderWidget<Gtk::Box> {
    private:
        std::shared_ptr<NBody::Simulation::Particle> _particle = {};

        ParticlePositionEntry &_positionEntry;
        ParticleVelocityEntry &_velocityEntry;

        ParticleMassCheckButton &_massCheckButton;
        ParticleMassEntry &_massEntry;

        ParticleAccelerationCheckButton &_accelerationCheckButton;
        ParticleAccelerationEntry &_accelerationEntry;

        ParticleColorEntry &_colorEntry;
        ParticleRadiusEntry &_radiusEntry;

    public:

        ParticleEntry();

        void bind(std::shared_ptr<NBody::Simulation::Particle> &particle);

        std::shared_ptr<NBody::Simulation::Particle> currentParticle();
    };

}

#endif //N_BODY_PARTICLEENTRY_H
