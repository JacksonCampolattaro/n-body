//
// Created by jackcamp on 9/6/20.
//

#ifndef N_BODY_COLOR_H
#define N_BODY_COLOR_H

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <Magnum/Magnum.h>
#include <Magnum/Math/Color.h>

#include <iomanip>

namespace Model {
    namespace Drawable {

        class Color : public Magnum::Color3 {
        public:

            Color(float r, float g, float b) : Magnum::Color3(r, g, b) {}

            template<typename Writer>
            friend Writer &operator<<(Writer &writer, const Color &color) {
                writer.StartObject();
                {
                    writer.String("r");
                    writer.Double(color.r());
                    writer.String("g");
                    writer.Double(color.g());
                    writer.String("b");
                    writer.Double(color.b());
                }
                writer.EndObject();
                return writer;
            }

            friend const rapidjson::Value &operator>>(const rapidjson::Value &obj, Color &color) {

                color.r() = obj["r"].GetFloat();
                color.g() = obj["g"].GetFloat();
                color.b() = obj["b"].GetFloat();

                return obj;
            }

            friend std::ostream &operator<<(std::ostream &os, const Color &color) {
                os << "( " << color.r() << "r " << color.g() << "g " << color.b() << "b )";
                return os;
            }
        };
    }
}

#endif //N_BODY_COLOR_H
