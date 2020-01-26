//
// Created by jackcamp on 1/26/20.
//

#include "Application.h"

Controller::Application::Application() : Gtk::Application {"my.app", Gio::APPLICATION_HANDLES_COMMAND_LINE} {

    add_main_option_entry(OptionType::OPTION_TYPE_BOOL, "silent", 's',
                          "Disables printing any information to the console");
}

int Controller::Application::on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine> &command_line) {
    auto options = command_line->get_options_dict();

    // Prepare the logger
    Logger::prepare(
            options->contains("silent")
    );

    return Gtk::Application::on_command_line(command_line);
}
