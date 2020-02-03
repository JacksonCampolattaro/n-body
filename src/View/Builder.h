//
// Created by jackcamp on 2/3/20.
//

#ifndef N_BODY_BUILDER_H
#define N_BODY_BUILDER_H

#include <gtkmm/builder.h>

namespace View {

    class Builder {
    public:

        Builder() {
            _builder = Gtk::Builder::create_from_file("viewer.glade");

        }

        Glib::RefPtr<Gtk::Builder> _builder;
    };
}

#endif //N_BODY_BUILDER_H
