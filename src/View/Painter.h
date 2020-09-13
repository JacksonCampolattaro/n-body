//
// Created by jackcamp on 9/12/20.
//

#ifndef N_BODY_PAINTER_H
#define N_BODY_PAINTER_H

#include <gtkmm/glarea.h>

#include "../Model/Drawable/Drawable.h"

namespace View {

    class Painter {

    public:

        // Setup

        Painter();

        void set_glarea(Gtk::GLArea &glArea);

        void draw(const std::deque<Model::Drawable::Drawable> &drawables);

        // Sinks

        void on_realize();

        bool on_render(const Glib::RefPtr<Gdk::GLContext> &context);

        void on_resize(int width, int height);

        void on_unrealize();
    };

}


#endif //N_BODY_PAINTER_H
