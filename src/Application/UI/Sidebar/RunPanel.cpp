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
                       NBody::Solver<Gravity> &solver, NBody::MultiRunner &runner) :
        BuilderWidget<Gtk::Box>(cobject, builder, "/ui/run_panel.xml"),
        _solver(solver),
        _multiRunner(runner),
        _runnerDropdown(getWidget<DropDownView>("run-mode-dropdown")),
        _statusLabel(getWidget<Gtk::Label>("status-label")),
        _runnerStack(getWidget<StackView>("run-controller-stack")),
        _iterationsLabel(getWidget<Gtk::Label>("iterations-label")),
        _stepTimeLabel(getWidget<PreciseFloatView>("step-time-label")),
        _averageStepTimeLabel(getWidget<PreciseFloatView>("average-step-time-label")),
        _computeTimeView(getWidget<TimeView>("compute-time-view")),
        _remainingTime(getWidget<Gtk::Box>("remaining-time")),
        _remainingTimeView(getWidget<TimeView>("remaining-time-view")) {

    _runnerDropdown.set_expression(Gtk::ClosureExpression<Glib::ustring>::create(
            [&](const Glib::RefPtr<Glib::ObjectBase> &item) {
                return std::dynamic_pointer_cast<NBody::Runner<Gravity>>(item)->name();
            }
    ));
    _runnerDropdown.set_model(_multiRunner.selectionModel());

    _runnerStack.set_expression(Gtk::ClosureExpression<Gtk::Widget *>::create(
            [&](const Glib::RefPtr<Glib::ObjectBase> &item) -> Gtk::Widget * {
                auto runner = std::dynamic_pointer_cast<NBody::Runner<Gravity>>(item);
                if (runner->id() == "continuous")
                    return new ContinuousRunnerController((NBody::ContinuousRunner<Gravity> &) *runner);
                if (runner->id() == "one-step")
                    return new OneStepRunnerController((NBody::OneStepRunner<Gravity> &) *runner);
                if (runner->id() == "n-steps")
                    return new NStepsRunnerController((NBody::NStepsRunner<Gravity> &) *runner);
                else return new Gtk::Label{"Unrecognized Runner"};
            }
    ));
    _runnerStack.set_model(_multiRunner.selectionModel());

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

//void UI::RunPanel::run(std::size_t n) {
//    spdlog::debug("Running {} steps", n);
//
//    _runnerDropdown.set_sensitive(false);
//
//    _currentIterations = 1;
//    _idler = Glib::signal_timeout().connect([this, n] {
//
//        _solver.slot_step()();
//
//        _stepCountEntry.set_progress_fraction((double) _currentIterations / (double) n);
//        auto averageDuration = std::reduce(_stepTimes.begin(), _stepTimes.end()) / _stepTimes.size();
//        _remainingTimeView.setValue(averageDuration * (n - _currentIterations));
//
//        if (_currentIterations >= n)
//            stop();
//
//        return true;
//    }, 1);
//}

//void UI::RunPanel::stop() {
//    _idler.disconnect();
//    _runContinuouslyButton.set_active(false);
//    //_runOneStepButton.set_active(false);
//    _runNStepsButton.set_active(false);
//    _stepCountEntry.set_progress_fraction(0.0);
//    _runnerDropdown.set_sensitive(true);
//}
