//
// Created by jackcamp on 6/6/19.
//

#ifndef N_BODY_RUNVIEWER_H
#define N_BODY_RUNVIEWER_H


#include <gtkmm/box.h>
#include <gtkmm/label.h>

class RunViewer : public Gtk::Box {

public:

    RunViewer();

    void on_completed_frame(int frameNumber);

private:

    Gtk::Label frameCount_label = Gtk::Label("0");
};


#endif //N_BODY_RUNVIEWER_H
