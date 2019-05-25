//
// Created by jackcamp on 5/22/19.
//

#ifndef N_BODY_RECORDERSETTINGSMENU_H
#define N_BODY_RECORDERSETTINGSMENU_H


#include <gtkmm/box.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/switch.h>
#include <gtkmm/togglebutton.h>
#include "../view/Recorder.h"

class RecorderSettingsMenu : public Gtk::Box {

public:

    RecorderSettingsMenu();
    virtual ~RecorderSettingsMenu();

    Recorder *getRecorder(View *view);

protected:

    void on_toggleRecording();

    Gtk::Box enableRecording_box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);
    Gtk::Label enableRecording_label = Gtk::Label("Enable Recording:");
    Gtk::Switch enableRecording_switch;

    Gtk::Box folderChooser_box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);
    Gtk::Label folderChooser_label = Gtk::Label("Output Folder:");
    Gtk::FileChooserButton folderChooser_button = Gtk::FileChooserButton("Select an Output Folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

};


#endif //N_BODY_RECORDERSETTINGSMENU_H
