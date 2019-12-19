//
// Created by jackcamp on 10/25/18.
//

#include "model/tools/BodyList.h"
#include "model/PhysicsContext.h"
#include "model/calculation/Solver.h"
#include "model/calculation/BarnesHut/BarnesHutSolver.h"
#include "model/calculation/Naive/NaiveSolver.h"

#include <fstream>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>

using std::cout;
using std::endl;

int main(int argc, char **argv) {

    // Loading from an xml file
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Loading Bodies
    std::ifstream bodiesInputStream("/home/jackcamp/CLionProjects/n_body/scenarios/blender/blender.bod");
    if (!bodiesInputStream.is_open())
        return 1;
    cereal::XMLInputArchive bodiesInputArchive(bodiesInputStream);
    auto bodies = BodyList();
    bodiesInputArchive(bodies);

    // Loading Physics
    std::ifstream physicsInputStream("/home/jackcamp/CLionProjects/n_body/scenarios/blender/blender.phys");
    if (!bodiesInputStream.is_open())
        return 2;
    cereal::XMLInputArchive physicsInputArchive(physicsInputStream);
    auto physics = PhysicsContext();
    physicsInputArchive(physics);


    // Creating a simulation from the loaded data
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Creating the solver, and configuring it
    Solver *solver = new NaiveSolver();
    solver->enableThreading();

    // Looping to run the simulation for many cycles
    for (int i = 0; i < 10000; ++i) {
        cout << "Solving moment " << i << endl;
        solver->solve(&bodies, &physics);
    }

    // Running the simulation for a number of cycles
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Saving the result to an xml file
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Saving Physics
    std::ofstream bodiesOutputStream("result.bod");
    cereal::XMLOutputArchive bodiesOutputArchive(bodiesOutputStream);
    bodiesOutputArchive(bodies);

    // Saving Bodies
    std::ofstream physicsOutputStream("test.phys");
    cereal::XMLOutputArchive physicsOutputArchive(physicsOutputStream);
    physicsOutputArchive(physics);


}