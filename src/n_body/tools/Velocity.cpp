//
// Created by jackcamp on 6/18/19.
//

#include <cereal/cereal.hpp>
#include "Velocity.h"


template<class Archive>
void Velocity::serialize(Archive & ar)
{
    ar(
            cereal::make_nvp("Δx", this->x), cereal::make_nvp("Δy", this->y), cereal::make_nvp("Δz", this->z)
    );
}