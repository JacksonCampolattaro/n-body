//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLESCOLUMNVIEW_H
#define N_BODY_PARTICLESCOLUMNVIEW_H

#include <gtkmm/box.h>
#include <gtkmm/columnview.h>

#include <NBody/Simulation/Simulation.h>

#include "ParticlesListModel.h"

#include "Bindable/ParticleActiveMassBindable.h"
#include "Bindable/ParticlePassiveMassBindable.h"
#include "Bindable/BindableListItemFactory.h"

namespace UI {

    class ParticlesColumnView : public Gtk::Box {
    private:

        NBody::Simulation &_simulation;
        Glib::RefPtr<ParticlesListModel> _particlesModel;

        Gtk::ScrolledWindow _scrolledWindow;
        Gtk::ColumnView _columnView;

    public:

        explicit ParticlesColumnView(NBody::Simulation &simulation);

    };

}

#endif //N_BODY_PARTICLESCOLUMNVIEW_H
