#ifndef N_BODY_ACTIVEMASS_H
#define N_BODY_ACTIVEMASS_H

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Physics {

    class ActiveMass {
    private:

        float _mass = 0;

    public:

        ActiveMass(float mass = 1.0f) : _mass(mass) {}

        [[nodiscard]] const float &mass() const { return _mass; };

        float &mass() { return _mass; };

        bool operator==(const ActiveMass &other) const;

        friend void to_json(json &j, const ActiveMass &m);

        friend void from_json(const json &j, ActiveMass &m);
    };

    void to_json(json &j, const ActiveMass &m);

    void from_json(const json &j, ActiveMass &m);

}

#endif //N_BODY_ACTIVEMASS_H
