//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_SPHERE_H
#define N_BODY_SPHERE_H

#include "Drawable.h"

namespace Model {

    namespace Drawable {

        class Sphere : public Drawable {
        public:

            Sphere(Color3 color, float radius);
        };

    }

}

#endif //N_BODY_SPHERE_H
