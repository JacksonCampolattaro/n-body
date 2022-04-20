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
                solver.step();
                return true;
            }, 1);
        else
            _idler.disconnect();
    });
}
