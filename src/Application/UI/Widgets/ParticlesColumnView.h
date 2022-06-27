//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLESCOLUMNVIEW_H
#define N_BODY_PARTICLESCOLUMNVIEW_H

#include <gtkmm/box.h>
#include <gtkmm/columnview.h>
#include <gtkmm/signallistitemfactory.h>
#include <gtkmm/singleselection.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/window.h>

#include <NBody/Simulation/Simulation.h>

#include "ParticlesListModel.h"

#include "Application/UI/Widgets/View/ParticleIconView.h"
#include "Application/UI/Widgets/View/ParticleActiveMassView.h"
#include "Application/UI/Widgets/View/ParticlePassiveMassView.h"
#include "Application/UI/Widgets/View/ParticlePositionView.h"
#include "Application/UI/Widgets/View/ParticleVelocityView.h"
#include "Application/UI/Widgets/View/ParticleColorView.h"
#include "Application/UI/Widgets/View/ParticleRadiusView.h"
#include "BindableListItemFactory.h"

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
