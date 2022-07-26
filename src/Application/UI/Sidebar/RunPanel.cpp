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

UI::RunPanel::RunPanel(NBody::Solver &solver) :
        Panel("Run"),
        _solver(solver),
        _builder(Gtk::Builder::create_from_resource("/ui/run_panel.xml")),
        _runModeDropdown(*_builder->get_widget<Gtk::ListBoxRow>("run-mode-dropdown")),
        _statusLabel(*_builder->get_widget<Gtk::Label>("status-label")),
        _runContinuouslyButton(*_builder->get_widget<Gtk::ToggleButton>("run-continuously-button")),
        _runOneStepButton(*_builder->get_widget<Gtk::ToggleButton>("run-one-step-button")),
        _runNStepsButton(*_builder->get_widget<Gtk::ToggleButton>("run-n-steps-button")),
        _stepCountEntry(*_builder->get_widget<Gtk::Entry>("step-count-entry")),
        _runControllerStack(*_builder->get_widget<Gtk::Stack>("run-controller-stack")),
        _iterationsLabel(*_builder->get_widget<Gtk::Label>("iterations-label")),
        _stepTimeLabel(
                *Gtk::Builder::get_widget_derived<PreciseFloatView>(_builder, "step-time-label")
        ),
        _averageStepTimeLabel(
                *Gtk::Builder::get_widget_derived<PreciseFloatView>(_builder, "average-step-time-label")
        ) {

    auto root = _builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);
    _contents.set_expand();
    append(*_builder->get_widget<Gtk::ActionBar>("action-bar"));


    _runControllerStack.set_visible_child("continuous");
    _runModeDropdown.connect_property_changed("selected", [&]() {
        switch (_runModeDropdown.get_property<guint>("selected")) {
            case 0:
                _runControllerStack.set_visible_child("continuous");
                break;
            case 1:
                _runControllerStack.set_visible_child("one-step");
                break;
            case 2:
                _runControllerStack.set_visible_child("n-steps");
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
    });

    _solver.signal_status().connect([&](const NBody::Status &status){
        _statusLabel.set_text({status.begin()});
    });

    //_solver._statusDispatcher.emit({"stopped"});
}

void UI::RunPanel::run(std::size_t n) {
    spdlog::debug("Running {} steps", n);

    _currentIterations = 1;
    _idler = Glib::signal_timeout().connect([this, n] {

        _solver.slot_step()();

        _stepCountEntry.set_progress_fraction((double) _currentIterations / (double) n);

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
}
