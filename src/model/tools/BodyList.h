//
// Created by jackcamp on 9/29/19.
//

#ifndef N_BODY_BODYLIST_H
#define N_BODY_BODYLIST_H


#include "../Body.h"

#include <cereal/cereal.hpp>
#include <vector>

class BodyList : public std::vector<Body> {


    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(
                cereal::make_nvp("BodyList", this)
        );
    }
};


#endif //N_BODY_BODYLIST_H
