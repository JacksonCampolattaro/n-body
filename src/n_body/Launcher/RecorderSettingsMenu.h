//
// Created by jackcamp on 5/22/19.
//

#ifndef N_BODY_RECORDERSETTINGSMENU_H
#define N_BODY_RECORDERSETTINGSMENU_H


#include <gtkmm/box.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/filechooserbutton.h>
#include "../view/Recorder.h"

class RecorderSettingsMenu : public Gtk::Box {

public:

    RecorderSettingsMenu();
    virtual ~RecorderSettingsMenu();

    Recorder *getRecorder();

protected:

    Gtk::ToggleButton enableRecording_toggle;

    Gtk::FileChooserButton file_chooser;

};


#endif //N_BODY_RECORDERSETTINGSMENU_H
