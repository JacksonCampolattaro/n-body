//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLESLISTMODEL_H
#define N_BODY_PARTICLESLISTMODEL_H

#include <set>

#include <giomm/listmodel.h>
#include <spdlog/spdlog.h>

#include <NBody/Simulation/Simulation.h>

namespace UI {

    class ParticlesListModel : public Gio::ListModel, public Glib::Object {
    public:

        explicit ParticlesListModel(NBody::Simulation &simulation)
                : Glib::ObjectBase(typeid(ParticlesListModel)), Glib::Object(), _simulation(simulation) {

            auto validParticles = _simulation.view<NBody::Physics::Position>();
            _entities.insert(validParticles.begin(), validParticles.end());

            simulation.signal_particle_added.connect([&](NBody::Entity entity) {

                // Add the new particle to the set
                auto [it, success] = _entities.insert(entity);
                assert(success);

                // Notify the UI that a new particle has been added (and where)
                items_changed(std::distance(_entities.begin(), it), 0, 1);
            });

            simulation.signal_particle_removed.connect([&](NBody::Entity entity) {

                // Remove the particle from the set
                auto it = _entities.find(entity);
                auto position = std::distance(_entities.begin(), it);
                _entities.erase(it);

                // Notify the UI that an particle was removed, and where in the set it appeared
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
            return _entities.size();
            return _simulation.particleCount();
        };

        gpointer get_item_vfunc(guint position) override {
            auto it = _entities.begin();
            std::advance(it, position);
            return (new NBody::Simulation::Particle(_simulation, *it))->gobj();

            auto view = _simulation.view<NBody::Physics::Position>();
            if (position > view.size() || !_simulation.valid(view[position])) return {};
            return (new NBody::Simulation::Particle(_simulation, view[position]))->gobj();
        };

    protected:

        NBody::Simulation &_simulation;
        std::set<NBody::Entity> _entities;

    };

}


#endif //N_BODY_PARTICLESLISTMODEL_H
