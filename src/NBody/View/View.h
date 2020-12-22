#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Icosphere.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Trade/MeshData.h>

#include <NBody/Model/Drawable/Drawable.h>

using namespace Magnum;
using namespace Math::Literals;

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

        struct SphereInstanceData {
            Matrix4 transformation;
            Matrix3x3 normal;
            Color3 color;
        };

        GL::Mesh _sphereMesh{NoCreate};
        GL::Buffer _sphereInstanceBuffer{NoCreate};
        Containers::Array<SphereInstanceData> _sphereInstances;
        Shaders::Phong _shader{NoCreate};
        Matrix4 _projection;
        float _aspectRatio = 1;
    };

}

#endif //N_BODY_VIEW_H
