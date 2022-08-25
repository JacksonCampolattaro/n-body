//
// Created by jackcamp on 2/16/22.
//

#include "Sidebar.h"

#include <gdkmm/pixbuf.h>
#include <gtkmm/icontheme.h>

UI::Sidebar::Sidebar(NBody::ArcBallControllableCamera &camera, NBody::Simulation &simulation,
                     NBody::Physics::Rule &rule, NBody::MultiSolver &solver) :
        Gtk::Box(Gtk::Orientation::HORIZONTAL),
        _notebook(),
        _bodiesPanel(simulation),
        _physicsPanel(rule),
        _cameraPanel(camera),
        _solverPanel(solver),
        _runPanel(solver) {

    _notebook.set_tab_pos(Gtk::PositionType::LEFT);
    append(_notebook);

    auto iconTheme = Gtk::IconTheme::get_for_display(Gdk::Display::get_default());

    _bodiesIcon.set(iconTheme->lookup_icon("nbody-particles-symbolic", 128));
    _notebook.append_page(_bodiesPanel, _bodiesIcon);

    _physicsIcon.set(iconTheme->lookup_icon("nbody-physics-symbolic", 128));
    _notebook.append_page(_physicsPanel, _physicsIcon);

    _solverIcon.set(iconTheme->lookup_icon("nbody-solver-symbolic", 128));
    _notebook.append_page(_solverPanel, _solverIcon);

    _cameraIcon.set(iconTheme->lookup_icon("nbody-camera-symbolic", 128));
    _notebook.append_page(_cameraPanel, _cameraIcon);

    _runIcon.set(iconTheme->lookup_icon("nbody-run-symbolic", 128));
    _notebook.append_page(_runPanel, _runIcon);

}
