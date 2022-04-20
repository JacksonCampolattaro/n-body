//
// Created by jackcamp on 2/16/22.
//

#include "Sidebar.h"

#include <gdkmm/pixbuf.h>

UI::Sidebar::Sidebar(NBody::GtkmmArcBallCamera &camera, NBody::Simulation &simulation,
                     NBody::Physics::Rule &rule, NBody::Solver &solver) :
        Gtk::Box(Gtk::Orientation::HORIZONTAL),
        _notebook(),
        _bodiesPanel(simulation),
        _physicsPanel(rule),
        _cameraPanel(camera),
        _solverPanel(),
        _runPanel(solver) {

    _notebook.set_tab_pos(Gtk::PositionType::LEFT);
    append(_notebook);

    _bodiesIcon.set(Gdk::Pixbuf::create_from_resource("/NBody/icons/outliner_ob_pointcloud.svg", -1, 128));
    _notebook.append_page(_bodiesPanel, _bodiesIcon);

    _physicsIcon.set(Gdk::Pixbuf::create_from_resource("/NBody/icons/physics.svg", -1, 128));
    _notebook.append_page(_physicsPanel, _physicsIcon);

    _solverIcon.set(Gdk::Pixbuf::create_from_resource("/NBody/icons/memory.svg", -1, 128));
    _notebook.append_page(_solverPanel, _solverIcon);

    _cameraIcon.set(Gdk::Pixbuf::create_from_resource("/NBody/icons/view_camera.svg", -1, 128));
    _notebook.append_page(_cameraPanel, _cameraIcon);

    //    auto step = new Gtk::Button("step");
    //    step->signal_clicked().connect(solver.slot_step);
    //
    //    solver.signal_finished.connect([&] {
    //        camera.signal_changed.emit();
    //    });

    _runIcon.set(Gdk::Pixbuf::create_from_resource("/NBody/icons/tria_right.svg", -1, 128));
    _notebook.append_page(_runPanel, _runIcon);

}
