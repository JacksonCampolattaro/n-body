//
// Created by Jackson Campolattaro on 12/28/22.
//

#ifndef N_BODY_DROPDOWNVIEW_H
#define N_BODY_DROPDOWNVIEW_H

#include <gtkmm/dropdown.h>
#include <gtkmm/singleselection.h>
#include <gtkmm/builder.h>

namespace UI {

    class DropDownView : public Gtk::DropDown {
    public:

        DropDownView(Gtk::DropDown::BaseObjectType *cobject,
                     const Glib::RefPtr<Gtk::Builder> &builder);

        void set_model(const Glib::RefPtr<Gtk::SingleSelection>& model);

    };

}


#endif //N_BODY_DROPDOWNVIEW_H
