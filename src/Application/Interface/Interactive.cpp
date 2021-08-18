//
// Created by jackcamp on 8/12/21.
//

#include "Interactive.h"

Interface::Interactive::Interactive(Simulation &simulation) : Interface(simulation),
                                                              _camera{
                                                                      Vector3::zAxis(5.0f),
                                                                      {},
                                                                      Vector3::yAxis(),
                                                                      45.0_degf,
                                                                      Vector2i{400, 400}
                                                              },
                                                              _view(_camera, simulation),
                                                              _headerbar(),
                                                              _configurationPanel(simulation),
                                                              _paned(Gtk::Orientation::ORIENTATION_HORIZONTAL) {

    set_titlebar(_headerbar);
    _headerbar.set_show_close_button(true);
    {
        _headerbar.pack_end(_menubutton);
        _menubutton.set_direction(Gtk::ArrowType::ARROW_NONE);
        // todo
        _menubutton.show();
    }
    _headerbar.show();

    add(_paned);
    _paned.show();

    _paned.pack1(_view, true, false);
    _view.show();

    _paned.pack2(_configurationPanel, false, false);
    _configurationPanel.show();

}

//bool Interface::Interactive::on_keyPressEvent(GdkEventKey *event) {
//
//    float stepSize = 2.0;
//
//    if (event->keyval == GDK_KEY_Left) {
//        signal_moveCamera.emit(Matrix4::translation(Vector3::xAxis(stepSize)));
//    }
//
//    if (event->keyval == GDK_KEY_Right) {
//        signal_moveCamera.emit(Matrix4::translation(Vector3::xAxis(-stepSize)));
//    }
//
//    if (event->keyval == GDK_KEY_Up) {
//        if (event->state & GDK_CONTROL_MASK)
//            signal_moveCamera.emit(Matrix4::translation(Vector3::yAxis(-stepSize)));
//        else
//            signal_moveCamera.emit(Matrix4::translation(Vector3::zAxis(stepSize)));
//    }
//
//    if (event->keyval == GDK_KEY_Down) {
//        if (event->state & GDK_CONTROL_MASK)
//            signal_moveCamera.emit(Matrix4::translation(Vector3::yAxis(stepSize)));
//        else
//            signal_moveCamera.emit(Matrix4::translation(Vector3::zAxis(-stepSize)));
//    }
//
//    return false;
//}
