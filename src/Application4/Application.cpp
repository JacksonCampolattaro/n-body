#include <spdlog/spdlog.h>
#include <fstream>

#include "Application.h"
#include "UI/Simple.h"
#include "UI/Interactive.h"

Application::Application() :
        Gtk::Application(
                "com.github.JacksonCampolattaro.nbody",
                Gio::Application::Flags::HANDLES_OPEN
        ) {}

Glib::RefPtr<Application> Application::create() {
    spdlog::set_level(spdlog::level::trace);
    return Glib::make_refptr_for_instance(new Application());
}

void Application::on_activate() {
    spdlog::debug("activate");

    auto window = new UI::Interactive(_simulation);
    add_window(*window);
    window->present();
}

void Application::on_open(const Application::type_vec_files &files, const Glib::ustring &hint) {
    spdlog::debug("open");

    for (const auto &file : files) {
        spdlog::info("Loading scenario from file: {}", file->get_parse_name().c_str());

        // TODO: Temporary solution, this should be done in the background using read_async()
        std::ifstream scenario_data{file->get_parse_name().c_str()};
        json data = json::parse(scenario_data);
        from_json(data, _simulation);

        spdlog::debug("Successfully loaded scenario with {} particles", _simulation.size());
        spdlog::trace("Scenario contents:\n{}", data.dump(4));
    }

    activate();
}
