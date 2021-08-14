//
// Created by jackcamp on 8/15/21.
//

#include "PhysicsPane.h"

Interface::Panes::PhysicsPane::PhysicsPane() : Pane(),
                                               _box(Gtk::Orientation::ORIENTATION_VERTICAL),
                                               _gravity("G", 0.1, 2),
                                               _placeholder3("xxx", "buttontext") {

    _icon->set_from_resource("/NBody/icons/physics.svg");

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

                _listBox.append(_gravity);
                _gravity.show();

                _listBox.append(_placeholder3);
                _placeholder3.show();
            }
            _listBox.show();
        }
        _frame.show();

    }
    _box.show();
}
