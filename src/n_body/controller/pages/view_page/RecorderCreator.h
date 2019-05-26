//
// Created by jackcamp on 5/24/19.
//

#ifndef N_BODY_RECORDERCREATOR_H
#define N_BODY_RECORDERCREATOR_H


#include <gtkmm/box.h>
#include <gtkmm/switch.h>
#include <gtkmm/label.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/frame.h>
#include "../../../view/Recorder.h"

class RecorderCreator : public Gtk::Frame {

public:

    RecorderCreator();


    Recorder *createRecorder(View *view);


protected:

    void on_toggleRecording();


    Gtk::Label
            header_label = Gtk::Label("<b>Recorder Properties</b>"),
            description_label = Gtk::Label("<i>Fields here affect the mp4 output of the simulation.</i>");

    Gtk::Box
            main_box = Gtk::Box(Gtk::ORIENTATION_VERTICAL),
            enableRecording_box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL),
            folderChooser_box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);

    Gtk::Label
            enableRecording_label = Gtk::Label("Enable Recording:"),
            folderChooser_label = Gtk::Label("Output Folder:");


    Gtk::Switch
            enableRecording_switch;

    Gtk::FileChooserButton
            folderChooser_button =
            Gtk::FileChooserButton("Select an Output Folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
};


#endif //N_BODY_RECORDERCREATOR_H
