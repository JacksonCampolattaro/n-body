//
// Created by jackcamp on 2/21/22.
//

#ifndef N_BODY_SPHEREVIEW_H
#define N_BODY_SPHEREVIEW_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/aspectframe.h>

#include "NBody/Graphics/Color.h"
#include "NBody/Graphics/Sphere.h"

namespace UI {

    class SphereView : public Gtk::AspectFrame {
    private:

        Gtk::DrawingArea _drawingArea;

        NBody::Graphics::Color _color;
        float _radius;

    public:

        SphereView();

        void setValue(const NBody::Graphics::Color &color, const NBody::Graphics::Sphere &sphere);

    protected:

        void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
    };

}


#endif //N_BODY_SPHEREVIEW_H
