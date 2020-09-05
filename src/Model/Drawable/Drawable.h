//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_DRAWABLE_H
#define N_BODY_DRAWABLE_H

#include "../Position.h"

#include <Magnum/Magnum.h>
#include <Magnum/Math/Color.h>

using namespace Magnum;

namespace Model {

    namespace Drawable {

        class Drawable {
        public:

            Drawable(Color3 color, float radius);

            const Position &position() const;

            Color3 _color;
            float _radius;

            const Position *_position;

            friend std::ostream &operator<<(std::ostream &os, const Drawable &drawable) {
                os << "{ drawable: "
                   << "{ color: "
                   << drawable._color.r() << " "
                   << drawable._color.g() << " "
                   << drawable._color.b() << " "
                   << " }"
                   << "{ radius: "
                   << drawable._radius
                   << " }"
                   << " }";
                return os;
            }

            friend std::istream &operator>>(std::istream &in, Drawable &drawable) {
                return in;
            }
        };

    }

}

#endif //N_BODY_DRAWABLE_H
