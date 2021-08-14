//
// Created by jackcamp on 8/13/21.
//

#include "ConfigurationPanel.h"

Interface::ConfigurationPanel::ConfigurationPanel(Simulation &simulation) : Gtk::Notebook(),
                                                                            _bodiesPane(simulation) {

    set_tab_pos(Gtk::POS_LEFT);

    append_page(_bodiesPane, _bodiesPane.icon());
    _bodiesIcon.set_from_resource("/NBody/icons/outliner_ob_pointcloud.svg");
    _bodiesPane.show();

    append_page(_physicsPlaceholder, _physicsIcon);
    _physicsIcon.set_from_resource("/NBody/icons/physics.svg");
    _physicsPlaceholder.show();

    append_page(_solverPlaceholder, _solverIcon);
    _solverIcon.set_from_resource("/NBody/icons/memory.svg");
    _solverPlaceholder.show();

    append_page(_positionPlaceholder, _positionIcon);
    _positionIcon.set_from_resource("/NBody/icons/orientation_global.svg");
    _positionPlaceholder.show();

    append_page(_recordingPlaceholder, _recordingIcon);
    _recordingIcon.set_from_resource("/NBody/icons/view_camera.svg");
    _recordingPlaceholder.show();

    append_page(_runPlaceholder, _runIcon);
    _runIcon.set_from_resource("/NBody/icons/tria_right.svg");
    _runPlaceholder.show();
}
