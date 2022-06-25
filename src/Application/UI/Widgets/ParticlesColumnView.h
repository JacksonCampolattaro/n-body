//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLESCOLUMNVIEW_H
#define N_BODY_PARTICLESCOLUMNVIEW_H

#include <gtkmm/box.h>
#include <gtkmm/columnview.h>
#include <gtkmm/signallistitemfactory.h>
#include <gtkmm/singleselection.h>

#include <NBody/Simulation/Simulation.h>
#include <gtkmm/window.h>

#include "ParticlesListModel.h"

#include "Bindable/ParticleActiveMassBindable.h"
#include "Bindable/ParticlePassiveMassBindable.h"
#include "Bindable/ParticlePositionBindable.h"
#include "Bindable/ParticleVelocityBindable.h"
#include "Bindable/BindableListItemFactory.h"
#include "Application/UI/Widgets/Entry/ParticleEntry.h"

namespace UI {

    class ParticlesColumnView : public Gtk::Box {
    private:

        NBody::Simulation &_simulation;
        Glib::RefPtr<Gtk::SingleSelection> _selectionModel;
        Glib::RefPtr<ParticlesListModel> _particlesModel;

        Gtk::ScrolledWindow _scrolledWindow;
        Gtk::ColumnView _columnView;

        ParticleEntry _particleEntry;
        Gtk::Window _window;

    public:

        explicit ParticlesColumnView(NBody::Simulation &simulation);

    };

}

#endif //N_BODY_PARTICLESCOLUMNVIEW_H
