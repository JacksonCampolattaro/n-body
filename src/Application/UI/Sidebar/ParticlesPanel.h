//
// Created by jackcamp on 2/16/22.
//

#ifndef N_BODY_PARTICLESPANEL_H
#define N_BODY_PARTICLESPANEL_H

#include <NBody/Simulation/Simulation.h>

#include <gtkmm/listbox.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/switch.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/actionbar.h>
#include <gtkmm/filechooserdialog.h>

#include "../Widgets/LabeledWidget.h"

#include "Panel.h"
#include "Application/UI/Widgets/ParticlesColumnView.h"

namespace UI {

class ParticlesPanel : public Panel {
    private:

        NBody::Simulation &_simulation;

        UI::ParticlesColumnView _particlesView;
        Gtk::Window _particlesWindow;

        Gtk::FileChooserDialog _fileChooser;

    public:

        ParticlesPanel(NBody::Simulation &simulation);

        void on_file_dialog_response(int response_id);

    };

}


#endif //N_BODY_PARTICLESPANEL_H
