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

#include <CLI/CLI.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

using std::cout;
using std::endl;
using std::string;
using std::make_shared;

int main(int argc, char **argv) {

    // Create the log distributor, which will pass output to different sinks
    auto logDistributor = make_shared<spdlog::sinks::dist_sink_st>();

    // Create a logger using the distributor as the sink
    auto logger = make_shared<spdlog::logger>("log", logDistributor);
    spdlog::register_logger(logger);

    // Set the default level of the logger to debug
    logger->set_level(spdlog::level::debug);

    // Attach stdout to logger
    logDistributor->add_sink(make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger->log(spdlog::level::debug, "Sink console:stdout attached to logger");


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

    // Getting the verbose option
    bool verbose = false;
    CLIApplication.add_flag("-v,--verbose",
                            verbose,
                            "Enables more thorough logging, at the cost of performance"
    );

    // Getting the logfile path, with a default value
    string logPath = "../../logs/log.log";
    CLIApplication.add_option("-l,--logfile",
                              logPath,
                              "Sets the path to write log files to",
                              true
    );

    // Getting the body archive path, with a default value
    string bodyArchivePath = "../../scenarios/blender/blender.bod";
    CLIApplication.add_option("-b,--bodies",
                              bodyArchivePath,
                              "Sets the file path to read bodies from",
                              true
    )->check(CLI::ExistingFile);

    // Getting the physics archive path, with a default value
    string physicsArchivePath = "../../scenarios/blender/blender.phys";
    CLIApplication.add_option("-p,--physics",
                              physicsArchivePath,
                              "Sets the file path to read physics from",
                              true
    )->check(CLI::ExistingFile);

    // Setting the number of cycles to calculate
    unsigned int cycles = 100;
    CLIApplication.add_option("-c,--cycles",
                              cycles,
                              "Sets the number of calculation cycles to perform",
                              true
    );

    // Interpreting the input using the CLI macro
    CLI11_PARSE(CLIApplication, argc, argv);

    // Parsing options
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Enabling more verbosity
    if (verbose)
        logger->set_level(spdlog::level::trace);

    // Attaching the logfile as another sink
    logDistributor->add_sink(make_shared<spdlog::sinks::basic_file_sink_st>(logPath));
    logger->log(spdlog::level::debug, "Sink file:\"{}\" attached to logger", logPath);

    // Running with an interface
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!headless)
        return 1;

    // Setting logfile
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //logDistributor->add_sink(make_shared<spdlog::sinks::(logPath)>());

    // Loading from XML files
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Loading Bodies
    std::ifstream bodiesInputStream(bodyArchivePath);
    if (!bodiesInputStream.is_open())
        return 1;
    cereal::XMLInputArchive bodiesInputArchive(bodiesInputStream);
    auto bodies = BodyList();
    bodiesInputArchive(bodies);

    // Loading Physics
    std::ifstream physicsInputStream(physicsArchivePath);
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
    for (int i = 0; i < cycles; ++i) {
        spdlog::get("log")->trace("Starting cycle {} ({}% Complete)",
                                  i,
                                  100.0f * (float) i / (float) cycles);
        solver->solve(&bodies, &physics);
    }

    // End the timer
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // Output the elapsed time
    spdlog::get("log")->debug("Completed {} cycles in {} s",
                              cycles,
                              std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0f
    );
    spdlog::get("log")->debug("Averaged {} µs per cycle",
                              std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() /
                              (float) cycles
    );


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