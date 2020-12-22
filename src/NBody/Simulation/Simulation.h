//
// Created by jackcamp on 12/22/20.
//

#ifndef N_BODY_SIMULATION_H
#define N_BODY_SIMULATION_H

#include <iostream>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Point_set_3/IO.h>

using std::istream;
using std::ostream;

namespace NBody::Simulation {

    typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
    typedef Kernel::FT FT;
    typedef Kernel::Point_3 Point;
    typedef Kernel::Vector_3 Vector;
    typedef CGAL::Point_set_3<Point, Vector> Point_set;

    class Simulation {
    public:

        friend ostream &operator<<(ostream &output, const Simulation &s) {
            CGAL::write_ply_point_set(output, s._bodies);
            return output;
        }

        friend istream &operator>>(istream &input, Simulation &s) {
            CGAL::read_ply_point_set(input, s._bodies);
            return input;
        }

    private:

        Point_set _bodies;

    };

}

#endif //N_BODY_SIMULATION_H
