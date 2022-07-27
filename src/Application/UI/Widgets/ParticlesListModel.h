//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLESLISTMODEL_H
#define N_BODY_PARTICLESLISTMODEL_H

#include <giomm/listmodel.h>
#include <spdlog/spdlog.h>

#include <NBody/Simulation/Simulation.h>

namespace UI {

    class ParticlesListModel : public Gio::ListModel, public Glib::Object {
    public:

        explicit ParticlesListModel(NBody::Simulation &simulation)
                : Glib::ObjectBase(typeid(ParticlesListModel)), Glib::Object(), _simulation(simulation) {

            simulation.signal_particles_added.connect([&](std::size_t count) {

                // When new particles are added to the simulation, they always appear at the beginning of the list,
                // so we notify the list model of new items at position 0.
                items_changed(0, 0, count);
            });

            simulation.signal_particle_removed.connect([&](std::size_t position) {
                items_changed(position, 1, 0);
            });
        }

        static Glib::RefPtr<ParticlesListModel> create(NBody::Simulation &simulation) {
            return Glib::make_refptr_for_instance(new ParticlesListModel(simulation));
        }

        GType get_item_type_vfunc() override {
            return NBody::Simulation::Particle::get_type();
        };

        guint get_n_items_vfunc() override {
            return _simulation.size<NBody::Physics::Position>();
        };

        gpointer get_item_vfunc(guint position) override {
            auto view = _simulation.view<NBody::Physics::Position>();
            if (position > view.size()) return {};
            return (new NBody::Simulation::Particle(_simulation, view[position]))->gobj();
        };

    protected:

        NBody::Simulation &_simulation;

    };

}


#endif //N_BODY_PARTICLESLISTMODEL_H
