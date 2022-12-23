#include <spdlog/spdlog.h>
#include <fstream>

#include <glibmm/variant.h>
#include <glibmm/variantiter.h>
#include <glibmm/varianttype.h>

#include <adwaita.h>

#include "Application.h"
#include "UI/Simple.h"
#include "UI/Interactive.h"

Application::Application() :
        Gtk::Application(
                "com.github.JacksonCampolattaro.nbody",
                Gio::Application::Flags::HANDLES_OPEN
        ),
        _solver(_simulation, _rule),
        _fileManager(_simulation) {}

Glib::RefPtr<Application> Application::create() {
    spdlog::set_level(spdlog::level::debug);
    return Glib::make_refptr_for_instance(new Application());
}

void Application::on_activate() {

    Gtk::IconTheme::get_for_display(Gdk::Display::get_default())->add_resource_path("/NBody/icons/");

    _solver.select<NBody::BarnesHutSolver>();

    auto builder = Gtk::Builder::create_from_resource("/ui/interactive.xml");
    auto interactive = Gtk::Builder::get_widget_derived<UI::Interactive>(
            builder, "primary-window",
            _simulation, _rule, _solver, _fileManager
    );

    // Apply LibAdwaita styling
    adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_DEFAULT);

    add_window(*interactive);
    interactive->present();
}

void Application::on_open(const Application::type_vec_files &files, const Glib::ustring &hint) {

    for (const auto &file: files) {
        spdlog::info("Loading scenario from file: {}", file->get_parse_name().c_str());

        _fileManager.openPath(file);

        std::ifstream scenario_data{file->get_parse_name().raw()};
        json data = json::parse(scenario_data);
        if (data.contains("G"))
            _rule.g() = data["G"].get<float>();

        spdlog::debug("Successfully loaded scenario with {} particles", _simulation.size());
    }

    activate();
}
