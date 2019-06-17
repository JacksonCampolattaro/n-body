//
// Created by jackcamp on 6/12/19.
//

#include "SolverCreator.h"

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
    comboBox_solver.set_active_text("Barnes-Hut Algorithm");


    // Arranging the main box

    pack_start(header_label, Gtk::PACK_SHRINK);
    pack_start(description_label, Gtk::PACK_SHRINK);
    pack_start(comboBox_solver, Gtk::PACK_SHRINK);


    // Making sure everything is drawn

    show_all_children();

}

Solver *SolverCreator::createSolver() {
    return nullptr;
}

void SolverCreator::on_combo_changed() {

}
