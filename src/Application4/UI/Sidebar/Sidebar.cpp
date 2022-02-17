//
// Created by jackcamp on 2/16/22.
//

#include "Sidebar.h"

UI::Sidebar::Sidebar(NBody::GtkmmArcBallCamera &camera, NBody::Simulation &simulation) :
        Gtk::Box(Gtk::Orientation::HORIZONTAL),
        _notebook(),
        _bodiesPanel(simulation),
        _physicsPanel(),
        _cameraPanel(camera),
        _solverPanel() {

    _notebook.set_tab_pos(Gtk::PositionType::LEFT);
    append(_notebook);

    _bodiesIcon.set_from_resource("/NBody/icons/outliner_ob_pointcloud.svg");
    _notebook.append_page(_bodiesPanel, _bodiesIcon);

    _physicsIcon.set_from_resource("/NBody/icons/physics.svg");
    _notebook.append_page(_physicsPanel, _physicsIcon);

    _solverIcon.set_from_resource("/NBody/icons/memory.svg");
    _notebook.append_page(_solverPanel, _solverIcon);

    _cameraIcon.set_from_resource("/NBody/icons/orientation_global.svg");
    _notebook.append_page(_cameraPanel, _cameraIcon);

    _recordingIcon.set_from_resource("/NBody/icons/view_camera.svg");
    _notebook.append_page(_recordingPanel, _recordingIcon);

}
