#ifndef N_BODY_BODIESPANE_H
#define N_BODY_BODIESPANE_H

#include <gtkmm/box.h>
#include <gtkmm/listbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/button.h>
#include <gtkmm/separator.h>
#include <gtkmm/scrolledwindow.h>

#include <NBody/Simulation/Simulation.h>

#include "../LabeledWidget.h"

using NBody::Simulation::Simulation;

namespace Interface::Panes {

    class BodiesPane : public Gtk::ScrolledWindow {
    private:

        Simulation &_simulation;

        Gtk::Box _box;

        Gtk::Label _label{"Bodies"};
        Gtk::Frame _frame;
        Gtk::ListBox _listBox{};

        LabeledWidget<Gtk::Label> _bodyCount;
        Gtk::Separator _separator;
        LabeledWidget<Gtk::Button> _placeholder3;

    public:

        BodiesPane(Simulation &simulation) : Gtk::ScrolledWindow(),
                                             _box(Gtk::Orientation::ORIENTATION_VERTICAL),
                                             _simulation(simulation),
                                             _bodyCount("Number of Bodies", std::to_string(_simulation.size())),
                                             _placeholder3("its a button", "im a button!") {

            override_background_color(
                    Gtk::ListBoxRow().get_style_context()->get_background_color(Gtk::StateFlags::STATE_FLAG_PRELIGHT)
            );

            add(_box);
            _box.set_margin_top(12);
            _box.set_margin_bottom(12);
            _box.set_margin_left(12);
            _box.set_margin_right(12);
            {

                _box.pack_start(_label, false, false, 10);
                _label.set_xalign(0);
                _label.show();

                _box.pack_start(_frame);
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
            _box.show();
        }

    };
}


#endif //N_BODY_BODIESPANE_H
