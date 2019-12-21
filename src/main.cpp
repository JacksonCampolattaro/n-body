//
// Created by jackcamp on 10/25/18.
//

#include "model/tools/BodyList.h"
#include "model/PhysicsContext.h"
#include "model/calculation/Solver.h"
#include "model/calculation/BarnesHut/BarnesHutSolver.h"
#include "model/calculation/Naive/NaiveSolver.h"

#include <fstream>
#include <chrono>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <cli11/CLI11.hpp>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char **argv) {


    // Configuring CLI Input
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Setting the name and description
    CLI::App CLIApplication("Modular Integrator for N-body Interaction");

    // Getting the headless option
    bool headless = false;
    CLIApplication.add_flag("--headless",
                              headless,
                              "Runs the program without an interface or graphics"
                              );

    // Setting the body archive, with default value
    string bodyArchivePath = "/home/jackcamp/CLionProjects/n_body/scenarios/blender/blender.bod";
    CLIApplication.add_option("-b,--bodies",
                              bodyArchivePath,
                              "Sets the file path to read bodies from",
                              true
            )
            ->check(CLI::ExistingFile);

    // Setting the physics archive, with default value
    string physicsArchivePath = "/home/jackcamp/CLionProjects/n_body/scenarios/blender/blender.phys";
    CLIApplication.add_option("-p,--physics",
                              physicsArchivePath,
                              "Sets the file path to read physics from",
                              true
            )
            ->check(CLI::ExistingFile);

    CLI11_PARSE(CLIApplication, argc, argv);

    if (!headless)
        return 0;

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
    Solver *solver = new BarnesHutSolver();
    solver->enableThreading();


    // Running the simulation for a number of cycles
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Start the timer
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Looping to run the simulation for many cycles
    int cycles = 100000;
    for (int i = 0; i < cycles; ++i) {
        cout << "Starting cycle " << i << endl;
        solver->solve(&bodies, &physics);
    }

    // End the timer
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // Output the elapsed time
    std::cout << "Completed " << cycles << " cycles in "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0f << " s "
              << "( " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
              << " µs )" << std::endl;
    std::cout << "Averaged "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (float) cycles
              << " µs per cycle"
              << std::endl;


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