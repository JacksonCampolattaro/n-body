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

                os << "{ \"drawable\" : ";
                {

                    os << "{ \"color\" : ";
                    {
                        os << "\""
                           << drawable._color.x() << " "
                           << drawable._color.y() << " "
                           << drawable._color.z()
                           << "\" ";
                    }
                    os << "} ";

                    os << "{ \"radius\" : ";
                    {
                        os << drawable._radius << " ";
                    }
                    os << "} ";
                }
                os << "} ";

                return os;
            }

            friend std::istream &operator>>(std::istream &in, Drawable &drawable) {

                std::string _;

                // The first value should be the open bracket
                in >> _;
                assert("{" == _);
                {

                    // The next value should be the name
                    in >> std::quoted(_);
                    assert("drawable" == _);

                    // The next value should be the colon
                    in >> _;
                    assert(":" == _);

                    // TODO: Split this out
                    // The first value of the color should be the open bracket
                    in >> _;
                    assert("{" == _);
                    {

                        // The next value should be the name
                        in >> std::quoted(_);
                        assert("color" == _);

                        // The next value should be the colon
                        in >> _;
                        assert(":" == _);

                        // Get the value
                        std::string value;
                        in >> std::quoted(value);
                        std::stringstream(value) >> drawable._color.x() >> drawable._color.y() >> drawable._color.z();

                    }
                    // The last value should be the close bracket
                    in >> _;
                    assert("}" == _);

                    // TODO: Split this out
                    // The first value of the radius should be the open bracket
                    in >> _;
                    assert("{" == _);
                    {

                        // The next value should be the name
                        in >> std::quoted(_);
                        assert("radius" == _);

                        // The next value should be the colon
                        in >> _;
                        assert(":" == _);

                        // Get the value
                        in >> drawable._radius;

                    }
                    // The last value should be the close bracket
                    in >> _;
                    assert("}" == _);
                }
                // The last value should be the close bracket
                in >> _;
                assert("}" == _);

                return in;
            }
        };

    }

}

#endif //N_BODY_DRAWABLE_H
