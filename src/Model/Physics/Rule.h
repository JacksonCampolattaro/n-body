//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_RULE_H
#define N_BODY_RULE_H

#include "ActiveElement.h"
#include "PassiveElement.h"

namespace Model {

    namespace Physics {

        class Rule {
        public:

            Rule();

            void operator()(const ActiveElement &active, PassiveElement &passive);

            float _G;
            float _t;
        };

    }
}


#endif //N_BODY_RULE_H
