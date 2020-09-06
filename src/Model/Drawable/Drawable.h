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

            template<typename Writer>
            friend Writer &operator<<(Writer &writer, const Drawable &drawable) {
                writer.StartObject();
                {
                    writer.String("radius");
                    writer.Double(drawable._radius);
                }
                writer.EndObject();
            }

            friend const rapidjson::Value &operator>>(const rapidjson::Value &obj, Drawable &drawable) {

                drawable._radius = obj["radius"].GetFloat();

                return obj;
            }

            friend std::ostream &operator<<(std::ostream &os, const Drawable &drawable) {
                os << "* " << drawable._radius << "r " << " *";
                return os;
            }

            friend bool operator==(const Drawable &left, const Drawable &right) {
                // TODO: This could be improved
                return left._radius == right._radius && left._color == right._color;
            }
        };

    }

}

#endif //N_BODY_DRAWABLE_H
