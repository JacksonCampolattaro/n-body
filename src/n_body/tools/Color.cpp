//
// Created by jackcamp on 6/18/19.
//

#include <cereal/cereal.hpp>
#include "Color.h"


template<class Archive>
void Color::serialize(Archive & ar)
{
    ar(
            cereal::make_nvp("R", this->x), cereal::make_nvp("G", this->y), cereal::make_nvp("B", this->z)
    );
}
