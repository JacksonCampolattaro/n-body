//
// Created by Jackson Campolattaro on 8/30/22.
//

#include "VideoRecorder.h"

UI::VideoRecorder::VideoRecorder(Gtk::Box::BaseObjectType *cobject,
                                 const Glib::RefPtr<Gtk::Builder> &builder,
                                 NBody::Recorder &recorder) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/video_recorder.xml"),
        _stack(getWidget<Gtk::Stack>("recorder-stack")),
        _screenshotButton(getWidget<Gtk::Button>("screenshot-button")),
        _startRecordingButton(getWidget<Gtk::Button>("start-recording-button")),
        _xSize(getWidget<SimplePositiveSizeEntry>("x-size-entry")),
        _ySize(getWidget<SimplePositiveSizeEntry>("y-size-entry")),
        _frameRate(getWidget<SimplePositiveSizeEntry>("frame-rate-entry")),
        _fileChooserButton(getWidget<Gtk::Button>("file-chooser-button")),
        _stopRecordingButton(getWidget<Gtk::Button>("stop-recording-button")),
        _liveFrameCountLabel(getWidget<Gtk::Label>("live-frame-count-label")),
        _liveFrameRateLabel(getWidget<Gtk::Label>("live-frame-rate-label")),
        _liveRunTimeView(getWidget<TimeView>("live-play-time-view")),
        _liveResolutionLabel(getWidget<Gtk::Label>("live-resolution-label")),
        _liveUncompressedSizeLabel(getWidget<Gtk::Label>("live-uncompressed-size-label")) {

    _xSize.set_value(1920);
    _ySize.set_value(1080);

    _frameRate.set_value(24);

    _screenshotButton.signal_clicked().connect([&]() {
        auto image = recorder.takeImage({
                                                _xSize.get_value_as_int(),
                                                _ySize.get_value_as_int()
                                        });
        _imageFileChooser.save(image);
    });

    _startRecordingButton.signal_clicked().connect([&]() {

        recorder.startVideo(_videoFileChooser.get_file()->get_path(),
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
    _startRecordingButton.set_sensitive(false);

    _stopRecordingButton.signal_clicked().connect([&]() {
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
    });

    _fileChooserButton.signal_clicked().connect(sigc::mem_fun(_videoFileChooser, &VideoFileChooserDialog::show));
    _videoFileChooser.signal_fileSelected.connect([&]() {
        _fileChooserButton.set_label(_videoFileChooser.prettyPath(20));
        _startRecordingButton.set_sensitive(true);
    });
    _videoFileChooser.set_current_name("out.mp4");

}
