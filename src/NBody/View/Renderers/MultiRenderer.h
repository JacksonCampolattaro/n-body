//
// Created by Jackson Campolattaro on 7/24/22.
//

#ifndef N_BODY_MULTIRENDERER_H
#define N_BODY_MULTIRENDERER_H

#include <NBody/View/Renderer.h>
#include <NBody/View/Renderers/PhongRenderer.h>

namespace NBody {

    class MultiRenderer : public Renderer {
    private:

        Renderer *_activeRenderer;

    public:

        MultiRenderer() : _activeRenderer(&getRenderer<PhongRenderer>()) {}

        template<class RendererType>
        RendererType &getRenderer() {
            static RendererType renderer{};
            return renderer;
        }

        Renderer &activeRenderer() {
            return *_activeRenderer;
        }

        template<class RendererType>
        void select() {
            _activeRenderer = &getRenderer<RendererType>();
        }

        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix,
                  const NBody::Simulation &simulation) override {
            activeRenderer().draw(transformationMatrix, projectionMatrix, simulation);
        };
    };

}

#endif //N_BODY_MULTIRENDERER_H
