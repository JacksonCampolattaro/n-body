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
                             NBody::SolverRenderer<Gravity> &solverRenderer,
                             NBody::FieldRenderer<Gravity> &fieldRenderer,
                             NBody::Recorder &recorder) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/camera_panel.xml"),
        _positionEntry(getWidget<CompactPositionEntry>("camera-position-entry")),
        _directionEntry(getWidget<CompactDirectionEntry>("camera-direction-entry")),
        _zoomEntry(getWidget<FloatEntry>("camera-zoom-entry")),
        _backgroundColorEntry(getWidget<Gtk::ColorButton>("background-color-entry")),
        _rendererDropdown(getWidget<DropDownView>("renderer-dropdown")),
        _fieldOverlaySwitch(getWidget<Gtk::Switch>("field-overlay-switch")),
        _debugOverlaySwitch(getWidget<Gtk::Switch>("debug-overlay-switch")),
        _videoRecorder(getWidget<VideoRecorder>("video-recorder", recorder)) {

    _debugOverlaySwitch.set_active(solverRenderer.enabled());
    _debugOverlaySwitch.property_active().signal_changed().connect([&]() {
        solverRenderer.setEnabled(_debugOverlaySwitch.get_active());
    });

    _fieldOverlaySwitch.set_active(fieldRenderer.enabled());
    _fieldOverlaySwitch.property_active().signal_changed().connect([&]() {
        fieldRenderer.setEnabled(_fieldOverlaySwitch.get_active());
    });

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
        _backgroundColorEntry.set_rgba(
                Gdk::RGBA{"#212121"}
                //{0.12, 0.12, 0.1, 1.0}
                );
    else _backgroundColorEntry.set_rgba({0.9, 0.9, 0.92, 1.0});
    auto color = _backgroundColorEntry.get_rgba();
    camera.setBackgroundColor({color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()});

    _rendererDropdown.set_expression(Gtk::ClosureExpression<Glib::ustring>::create(
            [&](const Glib::RefPtr<Glib::ObjectBase> &item) {
                return std::dynamic_pointer_cast<NBody::SimulationRenderer>(item)->name();
            }
    ));
    _rendererDropdown.set_model(renderer.selectionModel());
}
