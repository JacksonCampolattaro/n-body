//
// Created by jackcamp on 8/19/21.
//

#ifndef N_BODY_CAMERAPANE_H
#define N_BODY_CAMERAPANE_H

#include <NBody/View/GtkmmArcBallCamera.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>

#include "Pane.h"
#include "../PositionWidget.h"

namespace Interface::Panes {

    class CameraPane : public Pane {
    private:

        NBody::GtkmmArcBallCamera &_camera;

        Gtk::Box _box;

        Gtk::Label _label{"Camera"};
        Gtk::Frame _frame;
        Gtk::ListBox _listBox;

        Position example{0, 0, 0};
        PositionWidget _position;

    public:

        CameraPane(NBody::GtkmmArcBallCamera &camera) :
                Pane(),
                _camera(camera),
                _box(Gtk::Orientation::ORIENTATION_VERTICAL),
                _position(example) {
            icon().set_from_resource("/NBody/icons/view_camera.svg");

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
                    _listBox.set_selection_mode(Gtk::SelectionMode::SELECTION_NONE);
                    {

                        _listBox.append(_position);
                        _listBox.get_row_at_index(_listBox.get_children().size() - 1)->set_activatable(false);
                        _position.show();

                    }
                    _listBox.show();
                }
                _frame.show();
            }
            _box.show();
        }

    };
}


#endif //N_BODY_CAMERAPANE_H
