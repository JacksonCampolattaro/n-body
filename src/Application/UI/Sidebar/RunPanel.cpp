//
// Created by Jackson Campolattaro on 20/04/2022.
//

#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <glibmm/main.h>
#include "RunPanel.h"

UI::RunPanel::RunPanel(NBody::Solver &solver) : Panel("Run") {

    auto builder = Gtk::Builder::create_from_resource("/ui/run_panel.xml");
    auto root = builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);

    _stepButton = builder->get_widget<Gtk::Button>("step-button");
    _stepButton->signal_clicked().connect(solver.slot_step());

    _runButton = builder->get_widget<Gtk::ToggleButton>("run-button");
    _runButton->signal_toggled().connect([&] {
        if (_runButton->get_active())
            _idler = Glib::signal_timeout().connect([&] {
                solver.slot_step()();
                return true;
            }, 1);
        else
            _idler.disconnect();
    });

    _iterationsLabel = builder->get_widget<Gtk::Label>("iterations-label");
    solver.signal_finished().connect([&] {
        _iterations++;
        _iterationsLabel->set_text(Glib::ustring::format(_iterations));
    });

    _stepTimeLabel = Gtk::Builder::get_widget_derived<FloatView<5>>(builder, "step-time-label");
    _averageStepTimeLabel = Gtk::Builder::get_widget_derived<FloatView<10>>(builder, "average-step-time-label");
    solver.signal_computeTime().connect([&](auto &duration) {
        _stepTimeLabel->setValue(duration.count());
        _stepTimes.emplace_back(duration);
        if (_stepTimes.size() > 1000) _stepTimes.pop_front();
        auto averageDuration = std::reduce(_stepTimes.begin(), _stepTimes.end()) / _stepTimes.size();
        _averageStepTimeLabel->setValue((float) averageDuration.count());
    });

}
