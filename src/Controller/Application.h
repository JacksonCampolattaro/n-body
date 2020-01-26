//
// Created by jackcamp on 1/26/20.
//

#ifndef N_BODY_APPLICATION_H
#define N_BODY_APPLICATION_H


#include <gtkmm/application.h>

namespace Controller {

    class Application : public Gtk::Application {
    public:

        Application() : Gtk::Application {"my.app"} {

            add_main_option_entry(Glib::OptionGroup::SlotOptionArgString(), "silent", 's', "Disables printing any information to the console");
        };

    protected:

    private:

        int cycles = 100;

    };

}


#endif //N_BODY_APPLICATION_H
