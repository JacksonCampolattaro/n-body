//
// Created by jackcamp on 1/26/20.
//

#ifndef N_BODY_APPLICATION_H
#define N_BODY_APPLICATION_H

#include "Logger.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <iostream>

namespace Controller {

    /**
     * @brief An extension of Gtkmm's application class with project-specific features.
     *
     * This extension of Gtk::Application makes instancing my project easier.
     * It parses extra command line instructions and uses them to determine which features to enable.
     */
    class Application : public Gtk::Application {
    public:

        /**
         * @brief The default constructor defines a number of CLI options.
         *
         * This constructor sets the application's name and
         * provides descriptions for the program's various CLI options.
         */
        Application();

    protected:

        int on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine> &command_line) override;

        void on_activate() override;

    private:

        std::shared_ptr<Gtk::Button> _button;
        std::shared_ptr<Gtk::Window> _window;
    };

}


#endif //N_BODY_APPLICATION_H
