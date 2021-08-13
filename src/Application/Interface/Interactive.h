//
// Created by jackcamp on 8/12/21.
//

#ifndef N_BODY_INTERACTIVE_H
#define N_BODY_INTERACTIVE_H

#include <NBody/Simulation/Simulation.h>
#include <NBody/View/View.h>

#include <Magnum/Math/Matrix4.h>

#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>
#include <gtkmm/paned.h>

#include "Interface.h"

namespace Interface {

    using NBody::Simulation::Simulation;

    class Interactive : public Interface {
    private:

        Gtk::Paned _paned;

        NBody::View _view;
        Gtk::Notebook _notebook;

        Gtk::Label _placeholder1, _placeholder2;

    public:

        Interactive(Simulation &simulation) : Interface(simulation),
                                              _view(simulation),
                                              _paned(Gtk::Orientation::ORIENTATION_HORIZONTAL),
                                              _placeholder1("1"), _placeholder2("2") {

            add(_paned);
            _paned.show();

            _paned.add(_view);
            _view.show();

            _paned.add(_notebook);
            _notebook.set_tab_pos(Gtk::POS_LEFT);
            _notebook.show();

            _notebook.append_page(_placeholder1, "1");
            _placeholder1.show();

            _notebook.append_page(_placeholder2, "2");
            _placeholder2.show();

            signal_key_press_event().connect(sigc::mem_fun(*this, &Interactive::on_keyPressEvent), false);

            signal_moveCamera.connect(_view.slot_moveCamera);
        }

    public:
        // Signals

        sigc::signal<void(Magnum::Matrix4)> signal_moveCamera;

    public:
        // Slots

        bool on_keyPressEvent(GdkEventKey *event) {

            float stepSize = 2.0;

            if (event->keyval == GDK_KEY_Left) {
                signal_moveCamera.emit(Matrix4::translation(Vector3::xAxis(stepSize)));
            }

            if (event->keyval == GDK_KEY_Right) {
                signal_moveCamera.emit(Matrix4::translation(Vector3::xAxis(-stepSize)));
            }

            if (event->keyval == GDK_KEY_Up) {
                if (event->state & GDK_CONTROL_MASK)
                    signal_moveCamera.emit(Matrix4::translation(Vector3::yAxis(-stepSize)));
                else
                    signal_moveCamera.emit(Matrix4::translation(Vector3::zAxis(stepSize)));
            }

            if (event->keyval == GDK_KEY_Down) {
                if (event->state & GDK_CONTROL_MASK)
                    signal_moveCamera.emit(Matrix4::translation(Vector3::yAxis(stepSize)));
                else
                    signal_moveCamera.emit(Matrix4::translation(Vector3::zAxis(-stepSize)));
            }

            return false;
        }

    };
}


#endif //N_BODY_INTERACTIVE_H
