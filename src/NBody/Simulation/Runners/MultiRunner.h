//
// Created by Jackson Campolattaro on 12/30/22.
//

#ifndef N_BODY_MULTIRUNNER_H
#define N_BODY_MULTIRUNNER_H

#include <typeinfo>

#include <giomm/liststore.h>
#include <gtkmm/singleselection.h>

#include "../Runner.h"
#include "ContinuousRunner.h"
#include "NStepsRunner.h"
#include "OneStepRunner.h"

namespace NBody {

    class MultiRunner {
    private:

        Glib::RefPtr<Gio::ListStore<NBody::Runner<Gravity>>> _runnerList{
                Gio::ListStore<NBody::Runner<Gravity>>::create()
        };
        Glib::RefPtr<Gtk::SingleSelection> _runnerSelection{Gtk::SingleSelection::create(_runnerList)};

    public:

        MultiRunner(Solver<Gravity> &solver) {

            // Add available runners
            _runnerList->append(Glib::make_refptr_for_instance(new ContinuousRunner(solver)));
            _runnerList->append(Glib::make_refptr_for_instance(new NStepsRunner(solver)));
            _runnerList->append(Glib::make_refptr_for_instance(new OneStepRunner(solver)));

            // One solver must always be selected
            _runnerSelection->set_can_unselect(false);

            //            _runnerSelection->signal_selection_changed().connect([&](guint, guint){
            //                auto &r = *_runnerSelection->get_selected_item();
            //                spdlog::debug("Selected runner type \"{}\" ({})", typeid(r).name(), _runnerSelection->get_selected());
            //            });
        }

        void select(guint index) {
            _runnerSelection->select_item(index, true);

            auto &r = *_runnerList->get_item(index);
            spdlog::debug("Selected runner type \"{}\" ({})", typeid(r).name(), _runnerSelection->get_selected());
        }

        template<class R>
        void select() {
            for (guint i = 0; i < _runnerList->get_n_items(); ++i) {
                auto &r = *_runnerList->get_item(i);
                if (typeid(R) == typeid(r)) {
                    select(i);
                    return;
                }
            }
            spdlog::error("Unrecognized solver type \"{}\"", typeid(R).name());
        }

        template<class R>
        R &get() {
            for (guint i = 0; i < _runnerList->get_n_items(); ++i) {
                auto &r = *_runnerList->get_item(i);
                if (typeid(R) == typeid(r)) {
                    return *std::dynamic_pointer_cast<R>(_runnerList->get_item(i));
                }
            }
            spdlog::error("Unrecognized runner type \"{}\"", typeid(R).name());
            return (R &) get(); // todo: use an exception here
        }

        Runner<Gravity> &get() {
            return *std::dynamic_pointer_cast<NBody::Runner<Gravity>>(_runnerSelection->get_selected_item());
        }

        Glib::RefPtr<Gtk::SingleSelection> &selectionModel() { return _runnerSelection; }

    };

}


#endif //N_BODY_MULTIRUNNER_H
