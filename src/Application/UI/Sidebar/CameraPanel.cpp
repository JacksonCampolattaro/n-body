//
// Created by jackcamp on 2/16/22.
//

#include "CameraPanel.h"

#include <adwaita.h>
#include <giomm/simpleactiongroup.h>

UI::CameraPanel::CameraPanel(Gtk::Box::BaseObjectType *cobject,
                             const Glib::RefPtr<Gtk::Builder> &builder,
                             NBody::ArcBallControllableCamera &camera,
                             NBody::MultiRenderer &renderer,
                             NBody::Recorder &recorder) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/camera_panel.xml"),
        _positionEntry(getWidget<CompactPositionEntry>("camera-position-entry")),
        _directionEntry(getWidget<CompactDirectionEntry>("camera-direction-entry")),
        _zoomEntry(getWidget<FloatEntry>("camera-zoom-entry")),
        _backgroundColorEntry(getWidget<Gtk::ColorButton>("background-color-entry")),
        _shaderDropdown(getWidget<Gtk::ListBoxRow>("shader-dropdown")),
        _videoRecorder(getWidget<VideoRecorder>("video-recorder", recorder)) {

    camera.signal_changed().connect([&]() {
        _zoomEntry.setValue(camera.getZoom());
        auto position = camera.getPosition();
        _positionEntry.setValue({position.x(), position.y(), position.z()});
        auto direction = camera.getDirection();
        _directionEntry.setValue({direction.x(), direction.y(), direction.z()});
    });
    camera.signal_changed().emit();

    _zoomEntry.signal_changed.connect([&](float z) {
        camera.setZoom(z);
    });

    _positionEntry.signal_changed.connect([&](float x, float y, float z) {
        camera.setPosition({x, y, z});
    });

    _directionEntry.set_sensitive(false);

    _backgroundColorEntry.set_use_alpha();
    _backgroundColorEntry.signal_color_set().connect([&]() {
        auto color = _backgroundColorEntry.get_rgba();
        camera.setBackgroundColor({color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()});
    });

    // Set the chosen background color based on the selected style
    if (adw_style_manager_get_dark(adw_style_manager_get_default()))
        _backgroundColorEntry.set_rgba({0.12, 0.12, 0.1, 1.0});
    else _backgroundColorEntry.set_rgba({0.9, 0.9, 0.92, 1.0});
    auto color = _backgroundColorEntry.get_rgba();
    camera.setBackgroundColor({color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()});

    renderer.select<NBody::InstancedPhongRenderer>();
    _shaderDropdown.connect_property_changed("selected", [&]() {
        auto id = _shaderDropdown.get_property<guint>("selected");
        switch (id) {
            case 0:
                renderer.select<NBody::InstancedPhongRenderer>();
                break;
            case 1:
                renderer.select<NBody::PhongRenderer>();
                break;
            case 2:
                renderer.select<NBody::InstancedFlatRenderer>();
                break;
            case 3:
                renderer.select<NBody::FlatRenderer>();
                break;
            default:
                spdlog::error("Unrecognized renderer selected");
                break;
        }
        camera.signal_changed().emit();
    });

}
