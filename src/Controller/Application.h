//
// Created by jackcamp on 1/26/20.
//

#ifndef N_BODY_APPLICATION_H
#define N_BODY_APPLICATION_H

#include "Logger.h"
#include <gtkmm/application.h>
#include <iostream>

namespace Controller {

    class Application : public Gtk::Application {
    public:

        Application() : Gtk::Application {"my.app", Gio::APPLICATION_HANDLES_COMMAND_LINE} {

            add_main_option_entry(OptionType::OPTION_TYPE_BOOL, "silent", 's',
                                  "Disables printing any information to the console");
        };

    protected:

        int on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine> &command_line) override {
            auto options = command_line->get_options_dict();

            std::cout << "command";

            if (options->contains("silent")) {
                std::cout << " silent mode";
            }

            activate();

            return Gtk::Application::on_command_line(command_line);
        }

    private:


    };

}


#endif //N_BODY_APPLICATION_H
