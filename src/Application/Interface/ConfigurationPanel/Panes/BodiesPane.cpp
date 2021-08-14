//
// Created by jackcamp on 8/14/21.
//

#include "BodiesPane.h"

Interface::Panes::BodiesPane::BodiesPane(Simulation &simulation) : Pane(),
                                                                   _box(Gtk::Orientation::ORIENTATION_VERTICAL),
                                                                   _simulation(simulation),
                                                                   _bodyCount("Number of Bodies",
                                                                              std::to_string(_simulation.size())),
                                                                   _placeholder3("its a button", "im a button!") {

    _icon->set_from_resource("/NBody/icons/outliner_ob_pointcloud.svg");

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
