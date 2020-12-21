#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Framebuffer.h>

#include <NBody/Model/Drawable/Drawable.h>

using namespace Magnum;

using namespace Model;

namespace View {

    class View : public Gtk::GLArea {
    public:

        View();

        void setDrawables(const std::deque<Drawable::Drawable> *drawables);

    protected:

        virtual void draw(GL::Framebuffer &framebuffer);

    private:

        void setupGL();

        bool render(const Glib::RefPtr<Gdk::GLContext> &context);

        void resize(int width, int height);

    private:

        Platform::GLContext _context{NoCreate};

        const std::deque<Drawable::Drawable> *_drawables = nullptr;
    };

}

#endif //N_BODY_VIEW_H
