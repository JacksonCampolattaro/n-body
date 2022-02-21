//
// Created by jackcamp on 2/20/22.
//

#ifndef N_BODY_PARTICLEENTRY_H
#define N_BODY_PARTICLEENTRY_H

#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>

#include "PositionEntry.h"

#include <NBody/Simulation/Simulation.h>

namespace UI {

    class ParticleEntry : public Gtk::Grid {
    private:

        Glib::RefPtr<Gdk::Pixbuf> _pixbuf =
                Gdk::Pixbuf::create_from_resource("/NBody/icons/node_material.svg", -1, 256);

        Gtk::Image _icon;

        Gtk::Label
                _positionLabel{"P"},
                _velocityLabel{"V"},
                _massLabel{"M"};
        PositionEntry _positionEntry;

    public:

        ParticleEntry();

        void bind(const NBody::Simulation::Particle &particle);

    };

}

#endif //N_BODY_PARTICLEENTRY_H
