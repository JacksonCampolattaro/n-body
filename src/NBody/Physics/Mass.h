#ifndef N_BODY_MASS_H
#define N_BODY_MASS_H

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace NBody::Physics {

    struct ActiveTag {
    };

    struct PassiveTag {
    };

    class Mass {
    private:

        float _mass = 0;

    public:

        Mass(float mass = 1.0f) : _mass(mass) {}

        [[nodiscard]] const float &mass() const { return _mass; };

        float &mass() { return _mass; };

        bool operator==(const Mass &other) const;

        friend void to_json(json &j, const Mass &m);

        friend void from_json(const json &j, Mass &m);
    };

    void to_json(json &j, const Mass &m);

    void from_json(const json &j, Mass &m);

}

#endif //N_BODY_MASS_H
