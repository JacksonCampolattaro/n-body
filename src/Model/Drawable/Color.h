//
// Created by jackcamp on 9/6/20.
//

#ifndef N_BODY_COLOR_H
#define N_BODY_COLOR_H

#include <Magnum/Magnum.h>
#include <Magnum/Math/Color.h>

namespace Model {
    namespace Drawable {

        class Color : public Magnum::Color3 {
        public:

            Color(float r, float g, float b) : Magnum::Color3(r, g, b) {}
        };
    }
}

#endif //N_BODY_COLOR_H
