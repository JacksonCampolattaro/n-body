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
#include <gtkmm/numericsorter.h>
#include <gtkmm/expression.h>
#include <gtkmm/sortlistmodel.h>

#include <NBody/Simulation/Simulation.h>

#include "ParticlesListModel.h"

#include "Application/UI/Widgets/View/ParticleIDView.h"
#include "Application/UI/Widgets/View/ParticleIconView.h"
#include "Application/UI/Widgets/View/ParticleMassView.h"
#include "Application/UI/Widgets/View/ParticleTagView.h"
#include "Application/UI/Widgets/View/ParticlePositionView.h"
#include "Application/UI/Widgets/View/ParticleVelocityView.h"
#include "Application/UI/Widgets/View/ParticleColorView.h"
#include "Application/UI/Widgets/View/ParticleRadiusView.h"
#include "BindableListItemFactory.h"


namespace UI {

    class ParticlesColumnView : public Gtk::Box {
    private:

        NBody::Simulation &_simulation;
        Glib::RefPtr<Gtk::SingleSelection> _selectionModel;
        Glib::RefPtr<ParticlesListModel> _particlesModel;

        Gtk::ScrolledWindow _scrolledWindow;
        Gtk::ColumnView _columnView;

    public:

        explicit ParticlesColumnView(NBody::Simulation &simulation);

    public:

        sigc::signal<void(std::shared_ptr<NBody::Simulation::Particle> &)> signal_open_particle;

    };

}

#endif //N_BODY_PARTICLESCOLUMNVIEW_H
