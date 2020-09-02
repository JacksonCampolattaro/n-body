//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_DRAWABLE_H
#define N_BODY_DRAWABLE_H

#include "../Position.h"

#include <Magnum/Magnum.h>
#include <Magnum/Math/Color.h>

#include <memory>

using namespace Magnum;

namespace Model {

    namespace Drawable {

        class Drawable {
        public:

            Color3 _color;
            float _scale;
            const Position *_position;
        };

    }

}

#endif //N_BODY_DRAWABLE_H
