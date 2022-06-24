//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_BINDABLELISTITEMFACTORY_H
#define N_BODY_BINDABLELISTITEMFACTORY_H

#include <spdlog/spdlog.h>

#include <gtkmm/signallistitemfactory.h>

#include "NBody/Simulation/Simulation.h"

namespace UI {

    /**
     * A class is used here to effectively create a templated namespace.
     * This allows us to write `BindableListItemFactory<B>::create()`
     * instead of `BindableListItemFactory::create<B>()`.
     *
     * @tparam BindableType
     */
    template<typename BindableType>
    class BindableListItemFactory : public Gtk::SignalListItemFactory {
    public:

        BindableListItemFactory() = delete;

        static Glib::RefPtr<Gtk::SignalListItemFactory> create() {
            auto factory = Gtk::SignalListItemFactory::create();

            factory->signal_setup().connect([&](auto listItem) { on_setup(listItem); });
            factory->signal_bind().connect([&](auto listItem) { on_bind(listItem); });
            factory->signal_unbind().connect([&](auto listItem) { on_unbind(listItem); });
            factory->signal_teardown().connect([&](auto listItem) { on_teardown(listItem); });

            return factory;
        }

        static void on_setup(const Glib::RefPtr<Gtk::ListItem> &listItem) {
            spdlog::trace("Creating a new ListItem");
            listItem->set_child(*(new BindableType()));
        }

        static void on_bind(const Glib::RefPtr<Gtk::ListItem> &listItem) {
            spdlog::trace("Binding a ListItem");
            auto data = std::dynamic_pointer_cast<NBody::Simulation::Particle>(listItem->get_item());
            auto bindable = dynamic_cast<BindableType *>(listItem->get_child());
            if (data && bindable) {
                bindable->bind(data);
            }
        }

        static void on_unbind(const Glib::RefPtr<Gtk::ListItem> &listItem) {
            spdlog::trace("Un-Binding a ListItem");
            if (auto bindable = dynamic_cast<BindableType *>(listItem->get_child()))
                bindable->unbind();
        }

        static void on_teardown(const Glib::RefPtr<Gtk::ListItem> &listItem) {
            spdlog::trace("Deleting a ListItem");
            delete dynamic_cast<BindableType *>(listItem->get_child());
        }

    };

}

#endif //N_BODY_BINDABLELISTITEMFACTORY_H
