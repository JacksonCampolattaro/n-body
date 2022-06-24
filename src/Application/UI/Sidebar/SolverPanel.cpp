//
// Created by jackcamp on 2/17/22.
//

#include "SolverPanel.h"

UI::SolverPanel::SolverPanel() : Panel("Solver") {

    auto builder = Gtk::Builder::create_from_resource("/ui/solver_panel.xml");
    auto root = builder->get_widget<Gtk::Widget>("root");
    _contents.append(*root);
}
