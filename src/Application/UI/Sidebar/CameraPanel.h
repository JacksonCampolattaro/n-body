//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_CAMERAPANEL_H
#define N_BODY_CAMERAPANEL_H

#include <gtkmm/listbox.h>
#include <gtkmm/colorbutton.h>

#include <NBody/View/GtkmmArcBallCamera.h>
#include <NBody/View/Renderer.h>
#include <NBody/View/Renderers/PhongRenderer.h>
#include <NBody/View/Renderers/InstancedPhongRenderer.h>
#include <NBody/View/Renderers/FlatRenderer.h>
#include <NBody/View/Renderers/InstancedFlatRenderer.h>

#include "Panel.h"
#include "../Widgets/LabeledWidget.h"
#include "Application/UI/Widgets/Entry/VectorEntry.h"

namespace UI {

    class CameraPanel : public Panel {
    private:

        Glib::RefPtr<Gtk::Builder> _builder;

        CompactPositionEntry &_positionEntry;
        CompactDirectionEntry &_directionEntry;
        FloatEntry &_zoomEntry;
        Gtk::ColorButton &_backgroundColorEntry;
        Gtk::ListBoxRow &_shaderDropdown;

    public:

        CameraPanel(NBody::GtkmmArcBallCamera &camera);

    };

}


#endif //N_BODY_CAMERAPANEL_H
