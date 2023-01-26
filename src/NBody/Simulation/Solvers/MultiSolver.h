//
// Created by Jackson Campolattaro on 7/16/22.
//

#ifndef N_BODY_MULTISOLVER_H
#define N_BODY_MULTISOLVER_H

#include <NBody/Simulation/Solver.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/DualTreeSolver.h>
#include <NBody/Simulation/Solvers/DualTraversalSolver.h>

#include <gtkmm/singleselection.h>
#include <giomm/liststore.h>

namespace NBody {

    class MultiSolver : public NBody::Solver {
    private:

        Glib::RefPtr<Gio::ListStore<NBody::Solver>> _solverList{Gio::ListStore<NBody::Solver>::create()};
        Glib::RefPtr<Gtk::SingleSelection> _solverSelection{Gtk::SingleSelection::create(_solverList)};

        sigc::connection _currentStatusConnection;

    public:

        MultiSolver(Simulation &simulation, Physics::Rule &rule) :
                NBody::Solver(simulation, rule) {

            // Add available solvers
            _solverList->append(Glib::make_refptr_for_instance(new NBody::NaiveSolver(_simulation, _rule)));
            _solverList->append(Glib::make_refptr_for_instance(new NBody::BarnesHutSolver(_simulation, _rule)));
            _solverList->append(Glib::make_refptr_for_instance(new NBody::LinearBVHSolver(_simulation, _rule)));
            _solverList->append(Glib::make_refptr_for_instance(new NBody::MVDRSolver(_simulation, _rule)));
            _solverList->append(Glib::make_refptr_for_instance(new NBody::OctreeDualTraversalSolver(_simulation, _rule)));

            // One solver must always be selected
            _solverSelection->set_can_unselect(false);
        }

        std::string id() override { return get().id(); };

        std::string name() override { return get().name(); };

        void step() override { get().step(); };

        void select(guint index) {
            _solverSelection->select_item(index, true);
            _currentStatusConnection = get().signal_status().connect(signal_status().make_slot());
            spdlog::debug("Selected {} solver", get().name());
        }

        void select(const std::string &id) {
            for (guint i = 0; i < _solverList->get_n_items(); ++i) {
                if (id == _solverList->get_item(i)->id()) {
                    select(i);
                    return;
                }
            }
            spdlog::error("Unrecognized solver id \"{}\"", id);
        }

        NBody::Solver &get() {
            return *std::dynamic_pointer_cast<NBody::Solver>(_solverSelection->get_selected_item());
        }

        const NBody::Solver &get() const {
            return *std::dynamic_pointer_cast<NBody::Solver>(_solverSelection->get_selected_item());
        }

        template<typename SolverImplementation>
        SolverImplementation &get(const std::string &id) {
            for (int i = 0; i < _solverList->get_n_items(); ++i) {
                if (id == _solverList->get_item(i)->id()) {
                    return *std::dynamic_pointer_cast<SolverImplementation>(_solverList->get_item(i));
                }
            }
            spdlog::error("Unrecognized solver id \"{}\"", id);
            return (SolverImplementation &) get(); // todo: use an exception here
        }

        Glib::RefPtr<Gio::ListStore<NBody::Solver>> &listModel() { return _solverList; }

        Glib::RefPtr<Gtk::SingleSelection> &selectionModel() { return _solverSelection; }
    };

}

#endif //N_BODY_MULTISOLVER_H
