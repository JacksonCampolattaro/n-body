//
// Created by Jackson Campolattaro on 20/04/2022.
//

#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/actionbar.h>

#include <glibmm/main.h>

#include "RunPanel.h"

UI::RunPanel::RunPanel(NBody::Solver &solver) :
        Panel("Run"),
        _solver(solver),
        _builder(Gtk::Builder::create_from_resource("/ui/run_panel.xml")),
        _runModeDropdown(*_builder->get_widget<Gtk::ListBoxRow>("run-mode-dropdown")),
        _runButton(*_builder->get_widget<Gtk::ToggleButton>("run-button")),
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

    _runButton.signal_toggled().connect([&] {
        if (_runButton.get_active())
            run();
        else
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

}

void UI::RunPanel::run() {

    switch (_runModeDropdown.get_property<guint>("selected")) {
        case 0:
            run(std::numeric_limits<std::size_t>::max());
            break;
        case 1:
            run(100);
            break;
        case 2:
            run(1);
            break;
        default:;
            spdlog::error("Unrecognized algorithm selected");
            break;
    }
}

void UI::RunPanel::run(std::size_t n) {
    _currentIterations = 1;
    _idler = Glib::signal_timeout().connect([this, n] {
        _solver.slot_step()();
        if (_currentIterations >= n)
            stop();
        return true;
    }, 1);
}

void UI::RunPanel::stop() {
    _idler.disconnect();
    _runButton.set_active(false);
}
