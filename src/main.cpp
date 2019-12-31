//
// Created by jackcamp on 10/25/18.
//

///#include "runner.h"
#include "view/Viewport.h"
#include "model/PhysicsContext.h"
#include "runner.h"

#include <fstream>

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

    Viewport viewport({argc, argv});
    return viewport.exec();


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

    // Getting the debug option
    bool debug = false;
    CLIApplication.add_flag("-d,--debug",
                            debug,
                            "Enables logging of information that minimally impacts performance"
    );

    // Getting the quiet option
    bool quiet = false;
    CLIApplication.add_flag("-q,--quiet",
                            quiet,
                            "Disables printing debug information to the console"
    );

    // Getting the quiet option
    bool silent = false;
    CLIApplication.add_flag("-s,--silent",
                            quiet,
                            "Disables printing any information to the console"
    );

    // Getting the logfile path, with a default value
    string logPath = "";
    CLIApplication.add_option("-l,--logfile",
                              logPath,
                              "Sets the path to write log files to",
                              true
    );

    // Getting the body archive path, with a default value
    string bodyArchivePath = "../../../scenarios/blender/blender.bod";
    CLIApplication.add_option("-b,--bodies",
                              bodyArchivePath,
                              "Sets the file path to read bodies from",
                              true
    )->check(CLI::ExistingFile);

    // Getting the physics archive path, with a default value
    string physicsArchivePath = "../../../scenarios/blender/blender.phys";
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

    // Configuring logging
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Create the log distributor, which will pass output to different sinks
    auto logDistributor = make_shared<spdlog::sinks::dist_sink_st>();

    // Create a logger using the distributor as the sink
    auto logger = make_shared<spdlog::logger>("log", logDistributor);
    spdlog::register_logger(logger);

    // Set the default level of the logger to debug
    logger->set_level(spdlog::level::info);

    // Setting verbosity level
    if (debug)
        logger->set_level(spdlog::level::debug);
    if (verbose)
        logger->set_level(spdlog::level::trace);
    if (quiet)
        logger->set_level(spdlog::level::warn);

    // Attaching the console as a sink
    if (!silent) {
        logDistributor->add_sink(make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logger->log(spdlog::level::debug, "Sink console:stdout attached to logger");
    }

    // Attaching the logfile as another sink
    if (!logPath.empty()) {
        logDistributor->add_sink(make_shared<spdlog::sinks::basic_file_sink_mt>(logPath));
        logger->log(spdlog::level::debug, "Sink file:\"{}\" attached to logger", logPath);
    }

    // Loading from XML files
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Loading Bodies
    std::ifstream bodiesInputStream(bodyArchivePath);
    if (!bodiesInputStream.is_open()) {
        logger->error("Failed to load body file at path \"{}\"", bodyArchivePath);
        return 1;
    }
    cereal::XMLInputArchive bodiesInputArchive(bodiesInputStream);
    auto bodies = BodyList();
    bodiesInputArchive(bodies);

    // Loading Physics
    std::ifstream physicsInputStream(physicsArchivePath);
    if (!bodiesInputStream.is_open()) {
        logger->error("Failed to load physics file at path \"{}\"", physicsArchivePath);
        return 1;
    }
    cereal::XMLInputArchive physicsInputArchive(physicsInputStream);
    auto physics = PhysicsContext();
    physicsInputArchive(physics);


    // Running the simulation for a number of cycles
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!headless) {
        return runner::video(bodies, physics, cycles);
    }

    runner::headless(bodies, physics, cycles);


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