//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_CAMERAPANEL_H
#define N_BODY_CAMERAPANEL_H

#include <gtkmm/listbox.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/switch.h>

#include <NBody/View/ArcBallControllableCamera.h>
#include <NBody/View/Recorder.h>
#include <NBody/View/Renderers/PhongRenderer.h>
#include <NBody/View/Renderers/InstancedPhongRenderer.h>
#include <NBody/View/Renderers/FlatRenderer.h>
#include <NBody/View/Renderers/InstancedFlatRenderer.h>
#include <NBody/View/Renderers/VelocityRenderer.h>

#include <NBody/View/Renderers/SolverRenderer.h>

#include "Application/UI/Widgets/BuilderWidget.h"
#include "Application/UI/Widgets/Entry/VectorEntry.h"
#include "Application/UI/Widgets/Entry/IntEntry.h"
#include "Application/UI/Widgets/VideoRecorder.h"

namespace UI {

    class CameraPanel : public BuilderWidget<Gtk::Box> {
    private:

        Glib::RefPtr<Gtk::Builder> _builder;

        CompactPositionEntry &_positionEntry;
        CompactDirectionEntry &_directionEntry;
        FloatEntry &_zoomEntry;
        Gtk::ColorButton &_backgroundColorEntry;
        Gtk::ListBoxRow &_shaderDropdown;
        Gtk::Switch &_debugOverlaySwitch;

        VideoRecorder &_videoRecorder;

    public:

        CameraPanel(Gtk::Box::BaseObjectType *cobject,
                    const Glib::RefPtr<Gtk::Builder> &builder,
                    NBody::ArcBallControllableCamera &camera,
                    NBody::MultiRenderer &renderer,
                    NBody::SolverRenderer &solverRenderer,
                    NBody::Recorder &recorder);

    };

}


#endif //N_BODY_CAMERAPANEL_H
