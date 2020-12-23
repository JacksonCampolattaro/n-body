//
// Created by jackcamp on 12/22/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include <NBody/Simulation/EntityCollection.h>

#include <nlohmann/json.hpp>

#include <iostream>

using nlohmann::json;
using std::istream;
using std::ostream;

namespace NBody::Simulation {

    class Simulation {
    public:

        friend ostream &operator<<(ostream &out, const Simulation &s) {

            json j;
//            to_json(j, s);
            out << j;
            return out;
        }

        friend istream &operator>>(istream &in, Simulation &s) {

            json j;
            in >> j;
//            from_json(j, s);
            return in;
        }

        friend void to_json(json &j, const Simulation &s) {
//            j["bodies"] = {s._entityCollection};
        }

        friend void from_json(const json &j, Simulation &s) {
//            j.at("bodies").get_to(s._entityCollection);
            s._entityCollection = j.at("bodies").get<NBody::Simulation::EntityCollection>();
        }

    private:

        EntityCollection _entityCollection;
    };

}

#endif //N_BODY_SIMULATION_H
