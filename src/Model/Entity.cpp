//
// Created by jackcamp on 9/3/20.
//

#include "Entity.h"

#include "Simulation.h"

Model::Entity::Entity(Simulation &simulation, const Position &position, const Velocity &velocity) :
        _simulation(simulation),
        _position{_simulation._positions, position},
        _velocity{_simulation._velocities, velocity} {

}
