//
// Created by jackcamp on 2/16/22.
//

#include "CameraPanel.h"

#include <adwaita.h>

UI::CameraPanel::CameraPanel(NBody::GtkmmArcBallCamera &camera) :
        Panel("Camera"),
        _builder(Gtk::Builder::create_from_resource("/ui/camera_panel.xml")),
        _positionEntry(*Gtk::Builder::get_widget_derived<CompactPositionEntry>(_builder, "camera-position-entry")),
        _directionEntry(*Gtk::Builder::get_widget_derived<CompactDirectionEntry>(_builder, "camera-direction-entry")),
        _zoomEntry(*Gtk::Builder::get_widget_derived<FloatEntry>(_builder, "camera-zoom-entry")),
        _backgroundColorEntry(*_builder->get_widget<Gtk::ColorButton>("background-color-entry")),
        _shaderDropdown(*_builder->get_widget<Gtk::ListBoxRow>("shader-dropdown")),
        _recordingXSizeEntry(*Gtk::Builder::get_widget_derived<SimpleSizeEntry>(_builder, "recording-x-size-entry")),
        _recordingYSizeEntry(*Gtk::Builder::get_widget_derived<SimpleSizeEntry>(_builder, "recording-y-size-entry")) {

    camera.setLagging(0.5);

    auto root = _builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);

    camera.signal_positionChanged.connect(_positionEntry.slot_changed);
    _positionEntry.signal_changed.connect(camera.slot_moveTo);

    camera.signal_directionChanged.connect(_directionEntry.slot_changed);
    // todo: Allow the user to modify the direction directly
    _directionEntry.set_sensitive(false);

    camera.signal_zoomChanged.connect(_zoomEntry.slot_changed);
    _zoomEntry.signal_changed.connect([&](float z) {
        camera.setZoom(z);
    });

    _backgroundColorEntry.signal_color_set().connect([&]() {
        camera.setBackgroundColor(_backgroundColorEntry.get_rgba());
    });

    // Set the chosen background color based on the selected style
    if (adw_style_manager_get_dark(adw_style_manager_get_default()))
        _backgroundColorEntry.set_rgba({0.12, 0.12, 0.1, 1.0});
    else _backgroundColorEntry.set_rgba({0.9, 0.9, 0.92, 1.0});
    camera.setBackgroundColor(_backgroundColorEntry.get_rgba());

    camera.renderer().select<NBody::InstancedPhongRenderer>();
    _shaderDropdown.connect_property_changed("selected", [&]() {
        auto renderer = _shaderDropdown.get_property<guint>("selected");
        switch (renderer) {
            case 0:
                camera.renderer().select<NBody::InstancedPhongRenderer>();
                break;
            case 1:
                camera.renderer().select<NBody::PhongRenderer>();
                break;
            case 2:
                camera.renderer().select<NBody::InstancedFlatRenderer>();
                break;
            case 3:
                camera.renderer().select<NBody::FlatRenderer>();
                break;
            default:
                spdlog::error("Unrecognized renderer selected");
                break;
        }
        camera.signal_changed.emit();
    });

    _recordingXSizeEntry.set_value(1920);
    _recordingYSizeEntry.set_value(1080);
}
