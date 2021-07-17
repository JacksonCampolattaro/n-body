//
// Created by jackcamp on 7/17/21.
//

#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include <gtkmm/glarea.h>

namespace NBody {

    class View : public Gtk::GLArea {
    private:

    public:

        View() : Gtk::GLArea() {

            set_auto_render();

            signal_realize().connect(sigc::mem_fun(this, &View::onRealize));
            signal_unrealize().connect(sigc::mem_fun(this, &View::onUnrealize));
            signal_render().connect(sigc::mem_fun(this, &View::onRender));
        };

        void onRealize() {
            make_current();

            glClearColor(0.9f, 1.0f, 1.0f, 1.0f);
        }

        void onUnrealize() {

        }

        bool onRender(const Glib::RefPtr<Gdk::GLContext> &context) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return false;
        }

    };
}


#endif //N_BODY_VIEW_H
