#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>

using namespace Magnum;

namespace View {

    class View : public Gtk::GLArea {
    public:

        View();

        void draw();

    private:

        void realize();

        bool render(const Glib::RefPtr<Gdk::GLContext> &context);

        void resize(int width, int height);

        void unrealize();

    private:

        Platform::GLContext _context{NoCreate};
    };

}

#endif //N_BODY_VIEW_H
