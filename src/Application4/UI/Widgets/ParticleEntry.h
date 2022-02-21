//
// Created by jackcamp on 2/20/22.
//

#ifndef N_BODY_PARTICLEENTRY_H
#define N_BODY_PARTICLEENTRY_H

#include <NBody/Simulation/Simulation.h>

#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>

#include "VectorView.h"
#include "LabeledWidget.h"
#include "SphereView.h"

namespace UI {

    class ParticleEntry : public Gtk::Box {
    private:

        Gtk::Grid _grid;

        Gtk::Image _icon;

        SphereView _sphereView;

        Gtk::Label
                _positionLabel{"P = "},
                _velocityLabel{"V = "},
                _massLabel{"M = "};

        PositionView _positionView;
        VelocityView _velocityView;
        Gtk::Label _massView{"0.00"};

    public:

        ParticleEntry();

        void bind(const NBody::Simulation::Particle &particle);

    };

}

#endif //N_BODY_PARTICLEENTRY_H
