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
        ) {}

Glib::RefPtr<Application> Application::create() {
    spdlog::set_level(spdlog::level::debug);
    return Glib::make_refptr_for_instance(new Application());
}

void Application::on_activate() {
    spdlog::debug("activate");

    auto builder = Gtk::Builder::create_from_resource("/ui/primary_window.xml");
    auto window = builder->get_widget<Gtk::Window>("primary-window");
    auto box = builder->get_widget<Gtk::Box>("main-box");
    auto interactive = new UI::Interactive(_simulation);
    box->append(*interactive);

    // Create a new window
    //auto window = new UI::Interactive(_simulation);

    // Apply our custom styling
    //auto css = Gtk::CssProvider::create();
    //css->load_from_resource("/NBody/stylesheet/style.css");
    //window->get_style_context()->add_provider_for_display(Gdk::Display::get_default(), css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Apply LibAdwaita styling
    adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_PREFER_DARK);

    add_window(*window);
    window->present();
}

void Application::on_open(const Application::type_vec_files &files, const Glib::ustring &hint) {
    spdlog::debug("open");

    for (const auto &file: files) {
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
