//
// Created by jackcamp on 9/2/20.
//

#ifndef N_BODY_ACTIVEELEMENT_H
#define N_BODY_ACTIVEELEMENT_H


#include "../Position.h"
#include "../Velocity.h"

namespace Model {
    namespace Physics {

        class ActiveElement {
        public:

            ActiveElement(float mass);

            const Position &position() const;

            float _mass;
            Position *_position;

            friend std::ostream &operator<<(std::ostream &os, const ActiveElement &activeElement) {

                os << "{ "
                   << "\"activeElement\" : "
                   << activeElement._mass
                   << " }";
                return os;
            }


            friend std::istream &operator>>(std::istream &in, ActiveElement &activeElement) {

                std::string _;

                // The first value should be the open bracket
                in >> _;
                assert("{" == _);
                {

                    // The next value should be the name
                    in >> std::quoted(_);
                    assert("activeElement" == _);

                    // The next value should be the colon
                    in >> _;
                    assert(":" == _);

                    // Get the value
                    in >> activeElement._mass;

                }
                // The last value should be the close bracket
                in >> _;
                assert("}" == _);

                return in;
            }
        };

    }
}


#endif //N_BODY_ACTIVEELEMENT_H
