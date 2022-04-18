//
// Created by jackcamp on 4/12/22.
//

#ifndef N_BODY_PASSIVEMASS_H
#define N_BODY_PASSIVEMASS_H

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Physics {

    class PassiveMass {
    private:

        float _mass = 0;

    public:

        PassiveMass(float mass = 1.0f) : _mass(mass) {}

        [[nodiscard]] const float &mass() const { return _mass; };

        float &mass() { return _mass; };

        bool operator==(const PassiveMass &other) const;

        friend void to_json(json &j, const PassiveMass &m);

        friend void from_json(const json &j, PassiveMass &m);
    };

    void to_json(json &j, const PassiveMass &m);

    void from_json(const json &j, PassiveMass &m);

}

#endif //N_BODY_PASSIVEMASS_H
