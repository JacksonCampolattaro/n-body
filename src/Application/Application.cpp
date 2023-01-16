#include <spdlog/spdlog.h>
#include <fstream>

#include <glibmm/variant.h>
#include <glibmm/variantiter.h>
#include <glibmm/varianttype.h>

#include <adwaita.h>

#include "Application.h"
#include "UI/Interactive.h"

Application::Application() :
        Gtk::Application(
                "com.github.JacksonCampolattaro.nbody",
                Gio::Application::Flags::HANDLES_OPEN
        ),
        _solver(_simulation, _rule),
        _runner(_solver),
        _fileManager(_simulation) {

    add_main_option_entry(
            OptionType::STRING,
            "verbosity",
            'v',
            "Selects a logging level",
            "{off|error|info|debug|trace}"
    );

    add_main_option_entry(
            OptionType::STRING,
            "solver",
            's',
            "Selects a solving algorithm",
            "{naive|barnes-hut}"
    );

    add_main_option_entry(
            OptionType::INT,
            "iterations",
            'i',
            "Runs the specified number of iterations",
            "<iterations>"
    );

    add_main_option_entry(
            OptionType::FILENAME,
            "output-file",
            'o',
            "Sets the file path to write result to",
            "<path>"
    );

    add_main_option_entry(
            OptionType::DOUBLE,
            "timestep",
            't',
            "Sets the base time step used for integration",
            "<time step>"
    );

    add_main_option_entry(
            OptionType::DOUBLE,
            "gravity",
            'G',
            "Sets the constant of gravity",
            "<gravitational constant>"
    );
}

Glib::RefPtr<Application> Application::create() {
    spdlog::set_level(spdlog::level::debug);
    return Glib::make_refptr_for_instance(new Application());
}

void Application::on_activate() {

    Gtk::IconTheme::get_for_display(Gdk::Display::get_default())->add_resource_path("/NBody/icons/");

    auto builder = Gtk::Builder::create_from_resource("/ui/interactive.xml");
    auto interactive = Gtk::Builder::get_widget_derived<UI::Interactive>(
            builder, "primary-window",
            _simulation, _runner, _rule, _solver, _fileManager
    );

    // Apply LibAdwaita styling
    adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_DEFAULT);

    add_window(*interactive);
    interactive->present();
}

int Application::on_handle_local_options(const Glib::RefPtr<Glib::VariantDict> &options) {

    if (options->contains("verbosity")) {
        Glib::ustring verbosity;
        options->lookup_value("verbosity", verbosity);
        spdlog::set_level(spdlog::level::from_str(verbosity.raw()));
        spdlog::debug("Log level: {}", spdlog::level::to_string_view(spdlog::get_level()));
    }

    if (options->contains("solver")) {
        Glib::ustring solverID;
        options->lookup_value("solver", solverID);
        _solver.select(solverID);

        spdlog::debug("Using {} solver", _solver.name());
    }

    if (options->contains("timestep")) {
        double timestep;
        options->lookup_value("timestep", timestep);
        NBody::Solver::timeStep() = (float) timestep;
        spdlog::debug("Time step set to {}", NBody::Solver::timeStep());
    }

    if (options->contains("gravity")) {
        double gravity;
        options->lookup_value("gravity", gravity);
        _rule.g() = (float) gravity;
        spdlog::debug("Gravitational constant set to {}", _rule.g());
    }

    if (options->contains("iterations")) {
        int iterations;
        options->lookup_value("iterations", iterations);
        spdlog::debug("Running {} iterations", iterations);
        _runner.select<NBody::NStepsRunner>();
        _runner.get<NBody::NStepsRunner>().start(iterations);
    }

    return Gtk::Application::on_handle_local_options(options);
}

void Application::on_open(const Application::type_vec_files &files, const Glib::ustring &hint) {

    _fileManager.close();

    for (const auto &file: files) {

        if (!_fileManager.canSave())
            _fileManager.openPath(file);
        else
            _fileManager.importFromPath(file);

        std::ifstream scenario_data{file->get_parse_name().raw()};
        json data = json::parse(scenario_data);
        if (data.contains("G"))
            _rule.g() = data["G"].get<float>();

    }

    activate();
}
