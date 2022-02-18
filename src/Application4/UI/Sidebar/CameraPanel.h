//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_CAMERAPANEL_H
#define N_BODY_CAMERAPANEL_H

#include <gtkmm/listbox.h>

#include <NBody/View/GtkmmArcBallCamera.h>

#include "Panel.h"
#include "../Widgets/LabeledWidget.h"
#include "../Widgets/PositionEntry.h"

namespace UI {

    class CameraPanel : public Panel {
    private:

        Gtk::ListBox _list;
        LabeledWidget<PositionEntry> _position;

    public:

        CameraPanel(NBody::GtkmmArcBallCamera &camera);

    };

}


#endif //N_BODY_CAMERAPANEL_H
