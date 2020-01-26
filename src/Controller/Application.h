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

        Application();

    protected:

        int on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine> &command_line) override;

    private:


    };

}


#endif //N_BODY_APPLICATION_H
