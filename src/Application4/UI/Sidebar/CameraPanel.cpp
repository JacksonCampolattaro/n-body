//
// Created by jackcamp on 2/16/22.
//

#include "CameraPanel.h"

UI::CameraPanel::CameraPanel(NBody::GtkmmArcBallCamera &camera) :
        Panel("Camera"),
        _camera(camera) {


    auto builder = Gtk::Builder::create_from_resource("/ui/camera_panel.xml");
    auto &root = *builder->get_widget<Gtk::Widget>("root");
    _contents.append(root);

    auto &position = *Gtk::Builder::get_widget_derived<PositionEditable>(builder, "CameraPositionEditable");
    camera.signal_positionChanged.connect(position.slot_changed);
    position.signal_changed.connect(camera.slot_moveTo);

    auto &direction = *Gtk::Builder::get_widget_derived<DirectionEditable>(builder, "CameraDirectionEditable");
    camera.signal_directionChanged.connect(direction.slot_changed);
    direction.set_sensitive(false);

    auto &colorButton = *builder->get_widget<Gtk::ColorButton>("BackgroundColorButton");
    colorButton.signal_color_set().connect([&](){
        auto color = colorButton.get_rgba();
        camera.setBackgroundColor(color);
    });
    colorButton.set_rgba({0.12, 0.12, 0.1, 1.0});
}

void UI::CameraPanel::on_backgroundColorChanged() {
    //auto color = _backgroundColor.widget().get_rgba();
    //_camera.setBackgroundColor(color);
}
