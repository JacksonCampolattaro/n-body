//
// Created by Jackson Campolattaro on 8/30/22.
//

#ifndef N_BODY_VIDEORECORDER_H
#define N_BODY_VIDEORECORDER_H

#include "BuilderWidget.h"
#include "Application/UI/Widgets/Entry/IntEntry.h"
#include "Application/UI/Widgets/View/TimeView.h"
#include "Application/UI/Windows/VideoFileChooserDialog.h"

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/stack.h>
#include <giomm/simpleactiongroup.h>

#include <NBody/View/Recorder.h>
#include <gtkmm/label.h>

namespace UI {

    class VideoRecorder : public BuilderWidget<Gtk::Box> {
    private:

        Glib::RefPtr<Gio::SimpleAction> _startAction;

        Gtk::Stack &_stack;

        SimplePositiveSizeEntry &_xSize;
        SimplePositiveSizeEntry &_ySize;
        SimplePositiveSizeEntry &_frameRate;
        Gtk::Button &_fileChooserButton;
        VideoFileChooserDialog _fileChooser;

        Gtk::Label &_liveFrameCountLabel;
        Gtk::Label &_liveFrameRateLabel;
        TimeView &_liveRunTimeView;
        Gtk::Label &_liveResolutionLabel;
        Gtk::Label &_liveUncompressedSizeLabel;

    public:

        VideoRecorder(Gtk::Box::BaseObjectType *cobject,
                      const Glib::RefPtr<Gtk::Builder> &builder,
                      NBody::Recorder &recorder);
    };

}


#endif //N_BODY_VIDEORECORDER_H
