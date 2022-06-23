//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_BINDABLELISTITEMFACTORY_H
#define N_BODY_BINDABLELISTITEMFACTORY_H

#include <gtkmm/signallistitemfactory.h>
#include "NBody/Simulation/Simulation.h"

namespace UI {

    template <typename BindableType>
    class BindableListItemFactory : public Gtk::SignalListItemFactory {
    public:

        BindableListItemFactory() = delete;

        static Glib::RefPtr<Gtk::SignalListItemFactory> create() {
            auto factory = Gtk::SignalListItemFactory::create();

            factory->signal_setup().connect([](auto listItem) {
                listItem->set_child(*Gtk::make_managed<BindableType>());
            });

            factory->signal_bind().connect([](auto listItem) {
                auto data = std::dynamic_pointer_cast<NBody::Simulation::Particle>(listItem->get_item());
                auto bindable = dynamic_cast<BindableType *>(listItem->get_child());
                if (data && bindable) {
                    bindable->bind(*data);
                }
            });

            return factory;
        }
    };

}

#endif //N_BODY_BINDABLELISTITEMFACTORY_H
