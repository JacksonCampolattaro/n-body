//
// Created by jackcamp on 12/22/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef CGAL::Point_set_3<Point> Point_set;

namespace NBody::Simulation {

    class Simulation {
    public:

    private:

        Point_set _bodies;

    };

}

#endif //N_BODY_SIMULATION_H
