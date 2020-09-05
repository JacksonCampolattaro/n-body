//
// Created by jackcamp on 9/2/20.
//

#ifndef N_BODY_PASSIVEELEMENT_H
#define N_BODY_PASSIVEELEMENT_H

#include "../Position.h"
#include "../Velocity.h"

namespace Model {
    namespace Physics {

        class PassiveElement {
        public:

            PassiveElement(float mass);

            const Position &position() const;

            Velocity &velocity();

            float _mass;
            Velocity *_velocity;
            const Position *_position;

            friend std::ostream &operator<<(std::ostream &os, const PassiveElement &passiveElement) {

                os << "{ "
                   << "\"passive\" : "
                   << passiveElement._mass
                   << " }";
                return os;
            }


            friend std::istream &operator>>(std::istream &in, PassiveElement &passiveElement) {

                std::string _;

                // The first value should be the open bracket
                in >> _;
                assert("{" == _);
                {

                    // The next value should be the name
                    in >> std::quoted(_);
                    assert("passive" == _);

                    // The next value should be the colon
                    in >> _;
                    assert(":" == _);

                    // Get the value
                    in >> passiveElement._mass;

                }
                // The last value should be the close bracket
                in >> _;
                assert("}" == _);

                return in;
            }
        };

    }
}


#endif //N_BODY_PASSIVEELEMENT_H
