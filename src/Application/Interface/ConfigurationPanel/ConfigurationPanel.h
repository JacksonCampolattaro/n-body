//
// Created by jackcamp on 8/13/21.
//

#ifndef N_BODY_CONFIGURATIONPANEL_H
#define N_BODY_CONFIGURATIONPANEL_H

#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>

namespace Interface {

    class ConfigurationPanel : public Gtk::Notebook {
    private:

        Gtk::Label _placeholder1, _placeholder2;

    public:

        ConfigurationPanel() {

            set_tab_pos(Gtk::POS_LEFT);

            append_page(_placeholder1, "1");
            _placeholder1.show();

            append_page(_placeholder2, "2");
            _placeholder2.show();
        }

    public:
        // Signals

    public:
        // Slots

    };
}


#endif //N_BODY_CONFIGURATIONPANEL_H
