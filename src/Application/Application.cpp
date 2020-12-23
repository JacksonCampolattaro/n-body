//
// Created by jackcamp on 1/26/20.
//

#include "Application.h"

#include "../NBody/Model/Simulation.h"

#include <giomm/notification.h>
#include <fstream>

Application::Application() :
        Gtk::Application{
                "com.github.JacksonCampolattaro.nbody",
                Gio::APPLICATION_HANDLES_COMMAND_LINE |
                Gio::APPLICATION_HANDLES_OPEN
        } {

    add_main_option_entry(OptionType::OPTION_TYPE_BOOL,
                          "silent",
                          's',
                          "Disables printing any information to the console"
    );

    add_main_option_entry(OptionType::OPTION_TYPE_FILENAME,
                          "verbosity",
                          'v',
                          "Set the verbosity level of the program's logging",
                          "{trace|debug|info|warn|error|critical}"
    );

    add_main_option_entry(OptionType::OPTION_TYPE_FILENAME,
                          "logfile",
                          'l',
                          "Set the file path to write a log file to",
                          "FILEPATH"
    );

    add_main_option_entry(OptionType::OPTION_TYPE_BOOL,
                          "headless",
                          '\0',
                          "Runs the program without any interface or visualization"
    );

    add_main_option_entry(OptionType::OPTION_TYPE_BOOL,
                          "viewer",
                          '\0',
                          "Runs the program with only a visualization and no interactivity"
    );

    // TODO: Temporary solution for loading a simulation file
    add_main_option_entry(OptionType::OPTION_TYPE_FILENAME,
                          "file",
                          'f',
                          "Set the file path to load a simulation from",
                          "FILEPATH"
    );
}

int Application::on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine> &command_line) {
    auto options = command_line->get_options_dict();

    Logger::reset();
//    _simulation = Model::Simulation();

    // Only attach the console to the logger if the silent flag isn't set
    if (!options->contains("silent"))
        Logger::attachConsole();

    // Set the log level based on the verbosity flag
    if (std::string verbosity; options->lookup_value("verbosity", verbosity))
        Logger::setVerbosity(verbosity);

    // Attach a log file according to the logfile flag
    if (std::string logfilePath; options->lookup_value("logfile", logfilePath))
        Logger::attachFile(logfilePath);

    // Run the program in headless mode if that flag is set
    if (options->contains("headless")) {
        spdlog::info("Program was run in headless mode");

        spdlog::debug("Creating a headless view");
    }

    // Run the program in viewer mode if that flag was set
    if (options->contains("viewer")) {
        spdlog::info("Program was run in viewer mode");

        spdlog::debug("Creating a viewer view");
        //_view = std::make_shared<View::ViewerView>(*this, _simulation);
        this->add_window(_viewport.window());
    }
//
//    // Otherwise, run the program in interactive mode
//    if (!_view) {
//        spdlog::info("Program was run in interactive mode");
//
//        spdlog::debug("Creating an interactive view");
//        _view = std::make_shared<View::Interactive>(*this);
//    }
//
//    // Connect all the signals
//    {
//        _simulation.signal_update_progress
//                .connect(sigc::mem_fun(_view.get(), &View::View::on_simulation_progress));
//        _simulation.signal_drawables_changed
//                .connect(sigc::mem_fun(_view.get(), &View::View::on_drawables_changed));
//        _simulation.signal_update_complete
//                .connect(sigc::mem_fun(_view.get(), &View::View::on_simulation_complete));
//
//        _view->signal_start_simulation
//                .connect(sigc::mem_fun(&_simulation, &Model::Simulation::update));
//    }

    // Load a file if one was provided
    if (std::string filePath; options->lookup_value("file", filePath))
        std::ifstream(filePath) >> _simulation;

    // Give the view access to the drawables
//    _viewport.slot_setDrawables(&_simulation._drawables);

    // Run the program itself
    spdlog::debug("Running the program");
    activate();
    return Gtk::Application::on_command_line(command_line);
}

void Application::on_open(const Gio::Application::type_vec_files &files, const Glib::ustring &hint) {

    spdlog::trace("on_open invoked");
}
