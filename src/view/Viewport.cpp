//
// Created by jackcamp on 12/26/19.
//

#include "Viewport.h"

Viewport::Viewport(const Arguments &arguments) :
        Magnum::Platform::Application(
                arguments,
                Configuration{}.setTitle("Magnum Primitives Example")
        ) {

    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
}

void Viewport::drawEvent() {

}
