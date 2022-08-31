//
// Created by Jackson Campolattaro on 8/30/22.
//

#include "VideoRecorder.h"

UI::VideoRecorder::VideoRecorder(Gtk::Box::BaseObjectType *cobject,
                                 const Glib::RefPtr<Gtk::Builder> &builder,
                                 NBody::Recorder &recorder) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/video_recorder.xml"),
        _stack(getWidget<Gtk::Stack>("recorder-stack")),
        _xSize(getWidget<SimplePositiveSizeEntry>("x-size-entry")),
        _ySize(getWidget<SimplePositiveSizeEntry>("y-size-entry")),
        _frameRate(getWidget<SimplePositiveSizeEntry>("frame-rate-entry")),
        _fileChooserButton(getWidget<Gtk::Button>("file-chooser-button")),
        _liveFrameCountLabel(getWidget<Gtk::Label>("live-frame-count-label")),
        _liveFrameRateLabel(getWidget<Gtk::Label>("live-frame-rate-label")),
        _liveRunTimeView(getWidget<TimeView>("live-play-time-view")),
        _liveResolutionLabel(getWidget<Gtk::Label>("live-resolution-label")),
        _liveUncompressedSizeLabel(getWidget<Gtk::Label>("live-uncompressed-size-label")) {

    _xSize.set_value(1920);
    _ySize.set_value(1080);

    _frameRate.set_value(24);

    _fileChooserButton.signal_clicked().connect(sigc::mem_fun(_fileChooser, &VideoFileChooserDialog::show));
    _fileChooser.signal_fileSelected.connect([&]() {
        _fileChooserButton.set_label(_fileChooser.prettyPath(20));
        _startAction->set_enabled(true);
    });
    _fileChooser.set_current_name("out.mp4");

    auto actionGroup = Gio::SimpleActionGroup::create();
    insert_action_group("recorder", actionGroup);

    actionGroup->add_action("screenshot", [&]() {
        recorder.takeImage({
                                   _xSize.get_value_as_int(),
                                   _ySize.get_value_as_int()
                           });
    });

    _startAction = actionGroup->add_action("start", [&]() {

        recorder.startVideo(_fileChooser.get_file()->get_path(),
                            {
                                    _xSize.get_value_as_int(),
                                    _ySize.get_value_as_int()
                            }, _frameRate.get_value_as_int());

        _liveFrameRateLabel.set_text(Glib::ustring::format(_frameRate.get_value_as_int()));
        _liveResolutionLabel.set_text(Glib::ustring::format(
                _xSize.get_value_as_int(), "×", _ySize.get_value_as_int()
        ));
        _stack.set_visible_child("recording");
    });
    _startAction->set_enabled(false);

    actionGroup->add_action("stop", [&]() {
        recorder.stopVideo();
        _stack.set_visible_child("not-started");
    });

    recorder.signal_recordedFrame.connect([&]() {

        auto frameCount = recorder.getFrameCount();
        auto liveRunTime = std::chrono::duration<float>(
                (float) recorder.getFrameCount() / (float) _frameRate.get_value_as_int());
        auto frameSize = _xSize.get_value_as_int() * _ySize.get_value_as_int();

        _liveFrameCountLabel.set_text(Glib::ustring::format(frameCount));
        _liveRunTimeView.setValue(liveRunTime);
        _liveUncompressedSizeLabel.set_text(Glib::ustring::format(
                frameCount * frameSize * 3 / 1000000
        ));
        //_fileChooser.get_file()->query_info()->get_attribute_uint64("standard::size");
    });
}
