#ifndef N_BODY_BODIESPANE_H
#define N_BODY_BODIESPANE_H

#include <gtkmm/box.h>
#include <gtkmm/listbox.h>
#include <gtkmm/frame.h>

#include <NBody/Simulation/Simulation.h>

using NBody::Simulation::Simulation;

namespace Interface::Panes {

    class BodiesPane : public Gtk::Box {
    private:

        Simulation &_simulation;

        Gtk::Label _label{"Bodies"};
        Gtk::Frame _frame;
        Gtk::ListBox _listBox{};

        Gtk::Label _placeholder1{"test1"};
        Gtk::Label _placeholder2{"test2"};

    public:

        BodiesPane(Simulation &simulation) : Gtk::Box(Gtk::Orientation::ORIENTATION_VERTICAL),
                                              _simulation(simulation) {

            set_margin_top(10);
            set_margin_bottom(10);
            set_margin_left(10);
            set_margin_right(10);

            pack_start(_label);
            _label.show();

            pack_start(_frame);
            {
                _frame.add(_listBox);
                _frame.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
                {
                    _listBox.append(_placeholder1);
                    _placeholder1.set_text("Number of Bodies: " + std::to_string(_simulation.size()));
                    _placeholder1.show();

                    _listBox.append(_placeholder2);
                    _placeholder2.show();
                }
                _listBox.show();
            }
            _frame.show();


        }

    };
}


#endif //N_BODY_BODIESPANE_H
