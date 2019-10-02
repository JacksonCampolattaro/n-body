//
// Created by jackcamp on 9/29/19.
//

#ifndef N_BODY_BODYLIST_H
#define N_BODY_BODYLIST_H


#include <vector>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include "../Body.h"

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
