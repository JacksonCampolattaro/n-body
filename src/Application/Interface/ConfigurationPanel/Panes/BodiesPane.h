#ifndef N_BODY_BODIESPANE_H
#define N_BODY_BODIESPANE_H

#include <gtkmm/box.h>
#include <gtkmm/listbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/button.h>

#include <NBody/Simulation/Simulation.h>
#include <gtkmm/separator.h>

#include "../LabeledWidget.h"

using NBody::Simulation::Simulation;

namespace Interface::Panes {

    class BodiesPane : public Gtk::Box {
    private:

        Simulation &_simulation;

        Gtk::Label _label{"Bodies"};
        Gtk::Frame _frame;
        Gtk::ListBox _listBox{};

        LabeledWidget<Gtk::Label> _bodyCount;
        Gtk::Separator _separator;
        LabeledWidget<Gtk::Button> _placeholder3;

    public:

        BodiesPane(Simulation &simulation) : Gtk::Box(Gtk::Orientation::ORIENTATION_VERTICAL),
                                             _simulation(simulation),
                                             _bodyCount("Number of Bodies", std::to_string(_simulation.size())),
                                             _placeholder3("its a button", "im a button!") {

            set_margin_top(10);
            set_margin_bottom(10);
            set_margin_left(10);
            set_margin_right(10);

            pack_start(_label, false, false, 10);
            _label.set_xalign(0);
            _label.show();

            pack_start(_frame);
            _frame.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
            {
                _frame.add(_listBox);
                {

                    _listBox.append(_bodyCount);
                    _bodyCount.show();

                    _listBox.append(_separator);
                    _separator.set_can_focus(false);
                    _separator.set_sensitive(false);

                    _listBox.append(_placeholder3);
                    _placeholder3.show();
                }
                _listBox.show();
            }
            _frame.show();


        }

    };
}


#endif //N_BODY_BODIESPANE_H
