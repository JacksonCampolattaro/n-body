//
// Created by jackcamp on 8/12/21.
//

#include "Interactive.h"

Interface::Interactive::Interactive(Simulation &simulation) : Interface(simulation),
                                                              _view(simulation),
                                                              _configurationPanel(simulation),
                                                              _paned(Gtk::Orientation::ORIENTATION_HORIZONTAL) {

    add(_paned);
    _paned.show();

    _paned.pack1(_view, true, false);
    _view.show();

    _paned.pack2(_configurationPanel, false, false);
    _configurationPanel.show();


    signal_key_press_event().connect(sigc::mem_fun(*this, &Interactive::on_keyPressEvent), false);

    signal_moveCamera.connect(_view.slot_moveCamera);
}

bool Interface::Interactive::on_keyPressEvent(GdkEventKey *event) {

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