//
// Created by Jackson Campolattaro on 20/04/2022.
//

#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/actionbar.h>

#include <glibmm/main.h>

#include "RunPanel.h"

static std::string validate(const std::string &in) {
    auto filtered = in;
    std::erase_if(filtered, [](const auto &c) {
        return !std::isdigit(c);
    });
    return filtered.empty() ? "1" : filtered;
}

UI::RunPanel::RunPanel(Gtk::Box::BaseObjectType *cobject,
                       const Glib::RefPtr<Gtk::Builder> &builder,
                       NBody::Solver &solver) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/run_panel.xml"),
        _solver(solver),
        _runModeDropdown(getWidget<Gtk::ListBoxRow>("run-mode-dropdown")),
        _statusLabel(getWidget<Gtk::Label>("status-label")),
        _runContinuouslyButton(getWidget<Gtk::ToggleButton>("run-continuously-button")),
        _runOneStepButton(getWidget<Gtk::ToggleButton>("run-one-step-button")),
        _runNStepsButton(getWidget<Gtk::ToggleButton>("run-n-steps-button")),
        _stepCountEntry(getWidget<Gtk::Entry>("step-count-entry")),
        _runControllerStack(getWidget<Gtk::Stack>("run-controller-stack")),
        _iterationsLabel(getWidget<Gtk::Label>("iterations-label")),
        _stepTimeLabel(getWidget<PreciseFloatView>("step-time-label")),
        _averageStepTimeLabel(getWidget<PreciseFloatView>("average-step-time-label")),
        _computeTimeView(getWidget<TimeView>("compute-time-view")),
        _remainingTime(getWidget<Gtk::Box>("remaining-time")),
        _remainingTimeView(getWidget<TimeView>("remaining-time-view")) {

    _runControllerStack.set_visible_child("continuous");
    _remainingTime.hide();
    _runModeDropdown.connect_property_changed("selected", [&]() {
        switch (_runModeDropdown.get_property<guint>("selected")) {
            case 0:
                _runControllerStack.set_visible_child("continuous");
                _remainingTime.hide();
                break;
            case 1:
                _runControllerStack.set_visible_child("one-step");
                _remainingTime.hide();
                break;
            case 2:
                _runControllerStack.set_visible_child("n-steps");
                _remainingTime.show();
                break;
            default:
                spdlog::error("Unrecognized run mode selected");
                break;
        }
    });

    _runContinuouslyButton.signal_toggled().connect([&] {
        if (_runContinuouslyButton.get_active())
            run(std::numeric_limits<std::size_t>::max());
        else
            stop();
    });

    _runOneStepButton.signal_toggled().connect([&] {
        if (_runOneStepButton.get_active())
            run(1);
        else
            stop();
    });

    _runNStepsButton.signal_toggled().connect([&] {
        if (_runNStepsButton.get_active()) {
            _stepCountEntry.set_text(validate(_stepCountEntry.get_text()));
            run(std::stoi(_stepCountEntry.get_text()));
        } else
            stop();
    });

    solver.signal_finished().connect([&] {
        _iterations++;
        _currentIterations++;
        _iterationsLabel.set_text(Glib::ustring::format(_iterations));
    });

    solver.signal_computeTime().connect([&](auto &duration) {

        _stepTimeLabel.setValue(duration.count());

        _stepTimes.emplace_back(duration);
        if (_stepTimes.size() > 1000) _stepTimes.pop_front();
        auto averageDuration = std::reduce(_stepTimes.begin(), _stepTimes.end()) / _stepTimes.size();
        _averageStepTimeLabel.setValue((float) averageDuration.count());

        _totalElapsedComputeTime += duration;
        _computeTimeView.setValue(_totalElapsedComputeTime);
    });

    _solver.signal_status().connect([&](const NBody::Status &status) {
        _statusLabel.set_text({status.begin()});
    });
}

void UI::RunPanel::run(std::size_t n) {
    spdlog::debug("Running {} steps", n);

    _runModeDropdown.set_sensitive(false);

    _currentIterations = 1;
    _idler = Glib::signal_timeout().connect([this, n] {

        _solver.slot_step()();

        _stepCountEntry.set_progress_fraction((double) _currentIterations / (double) n);
        auto averageDuration = std::reduce(_stepTimes.begin(), _stepTimes.end()) / _stepTimes.size();
        _remainingTimeView.setValue(averageDuration * (n - _currentIterations));

        if (_currentIterations >= n)
            stop();

        return true;
    }, 1);
}

void UI::RunPanel::stop() {
    _idler.disconnect();
    _runContinuouslyButton.set_active(false);
    _runOneStepButton.set_active(false);
    _runNStepsButton.set_active(false);
    _stepCountEntry.set_progress_fraction(0.0);
    _runModeDropdown.set_sensitive(true);
}
