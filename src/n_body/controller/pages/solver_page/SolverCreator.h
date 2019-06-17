//
// Created by jackcamp on 6/12/19.
//

#ifndef N_BODY_SOLVERCREATOR_H
#define N_BODY_SOLVERCREATOR_H


#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/spinbutton.h>
#include "../../../model/calculation/Solver.h"

class SolverCreator : public Gtk::Box {

public:

    SolverCreator();

    Solver *createSolver();

private:

    Gtk::Label
            header_label = Gtk::Label("<b>Solver Properties</b>"),
            description_label = Gtk::Label("<i>Set values here to tweak the algorithm used by the program.</i>");

    Gtk::ComboBoxText
            comboBox_solver;

    Gtk::SpinButton
            theta_spinButton;

    void on_combo_changed();




};


#endif //N_BODY_SOLVERCREATOR_H