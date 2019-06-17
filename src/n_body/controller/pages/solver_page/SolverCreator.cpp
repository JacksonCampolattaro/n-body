//
// Created by jackcamp on 6/12/19.
//

#include <iostream>
#include "SolverCreator.h"
#include "../../../model/calculation/BarnesHut/BarnesHutSolver.h"
#include "../../../model/calculation/Naive/NaiveSolver.h"

SolverCreator::SolverCreator() {

    // Configuring the main box

    set_orientation(Gtk::ORIENTATION_VERTICAL);
    set_margin_top(20);
    set_margin_bottom(20);
    set_margin_left(30);
    set_margin_right(30);
    set_spacing(10);


    // Configuring the header

    header_label.set_use_markup(true);
    header_label.set_alignment(Gtk::ALIGN_START);


    // Configuring the description

    description_label.set_use_markup(true);
    description_label.set_alignment(Gtk::ALIGN_START);
    description_label.set_margin_bottom(10);


    // Configuring the combo box

    solver_comboBox.append("Barnes-Hut Algorithm");
    solver_comboBox.append("Naive Algorithm");
    solver_comboBox.set_active(0);
    solver_comboBox.signal_changed().connect(sigc::mem_fun(*this, &SolverCreator::on_combo_changed));


    // Configuring the theta label

    theta_label.set_alignment(Gtk::ALIGN_START);


    // Configuring the theta spinButton

    theta_spinButton.set_range(0, 100);
    theta_spinButton.set_digits(3);
    theta_spinButton.set_increments(0.01, 0.1);
    theta_spinButton.set_value(0.90);
    theta_spinButton.set_snap_to_ticks(false);


    // Configuring the theta box
    theta_box.pack_start(theta_label);
    theta_box.pack_start(theta_spinButton);


    // Configuring the threading label

    threading_label.set_alignment(Gtk::ALIGN_START);


    // Configuring the threading switch

    threading_switch.set_active(true);


    // Configuring the threading box

    threading_box.pack_start(threading_label);
    threading_box.pack_start(threading_switch, Gtk::PACK_SHRINK);


    // Arranging the main box

    pack_start(header_label, Gtk::PACK_SHRINK);
    pack_start(description_label, Gtk::PACK_SHRINK);
    pack_start(solver_comboBox, Gtk::PACK_SHRINK);
    pack_start(threading_box, Gtk::PACK_SHRINK);
    pack_start(theta_box, Gtk::PACK_SHRINK);


    // Making sure everything is drawn

    show_all_children();

}

Solver *SolverCreator::createSolver() {

    if (0 == solver_comboBox.get_active_row_number()) {
        // If Barnes Hut is chosen

        auto solver = new BarnesHutSolver;
        solver->setTheta(theta_spinButton.get_value());
        solver->enableThreading(threading_switch.get_active());
        return solver;

    } else {
        // If Naive is chosen

        auto solver = new NaiveSolver;
        solver->enableThreading(threading_switch.get_active());
        return solver;
    }
}

void SolverCreator::on_combo_changed() {

    if (0 == solver_comboBox.get_active_row_number()) {
        // If Barnes Hut is chosen
        theta_box.show();

    } else {
        // If Naive is chosen
        theta_box.hide();
    }

}
