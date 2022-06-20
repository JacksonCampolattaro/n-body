#include <spdlog/spdlog.h>
#include <fstream>

#include <adwaita.h>

#include "Application.h"
#include "UI/Simple.h"
#include "UI/Interactive.h"

Application::Application() :
        Gtk::Application(
                "com.github.JacksonCampolattaro.nbody",
                Gio::Application::Flags::HANDLES_OPEN
        ), _solver(_simulation, _rule) {}

Glib::RefPtr<Application> Application::create() {
    spdlog::set_level(spdlog::level::debug);
    return Glib::make_refptr_for_instance(new Application());
}

void Application::on_activate() {
    spdlog::debug("activate");

    Gtk::IconTheme::get_for_display(Gdk::Display::get_default())->add_resource_path("/NBody/icons/");

    auto builder = Gtk::Builder::create_from_resource("/ui/interactive.xml");
    auto interactive = Gtk::Builder::get_widget_derived<UI::Interactive>(
            builder, "primary-window",
            _simulation, _rule, _solver
    );

    // Apply LibAdwaita styling
    adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_DEFAULT);

    add_window(*interactive);
    interactive->present();
}

void Application::on_open(const Application::type_vec_files &files, const Glib::ustring &hint) {
    spdlog::debug("open");

    for (const auto &file: files) {
        spdlog::info("Loading scenario from file: {}", file->get_parse_name().c_str());

        // TODO: Temporary solution, this should be done in the background using read_async()
        std::ifstream scenario_data{file->get_parse_name().c_str()};
        json data = json::parse(scenario_data);
        from_json(data, _simulation);

        if (data.contains("G"))
            _rule.g() = data["G"].get<float>();

        spdlog::debug("Successfully loaded scenario with {} particles", _simulation.size());
        spdlog::trace("Scenario contents:\n{}", data.dump(4));
    }

    activate();
}
