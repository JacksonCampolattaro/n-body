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
            _liveEntities = {validParticles.begin(), validParticles.end()};
            std::sort(_liveEntities.begin(), _liveEntities.end());

            simulation.signal_particles_added.connect(sigc::mem_fun(*this, &ParticlesListModel::add_particles));

            simulation.signal_particle_removed.connect([&](NBody::Entity entity) {

                // Remove the particle from the set
                auto iterator = std::find(_liveEntities.begin(), _liveEntities.end(), entity);
                auto index = iterator - _liveEntities.begin();
                _liveEntities.erase(iterator);

                // Notify the UI that an particle was removed, and where in the set it appeared
                items_changed(index, 1, 0);
            });
        }

        static Glib::RefPtr<ParticlesListModel> create(NBody::Simulation &simulation) {
            return Glib::make_refptr_for_instance(new ParticlesListModel(simulation));
        }

        GType get_item_type_vfunc() override {
            return NBody::Simulation::Particle::get_type();
        };

        guint get_n_items_vfunc() override {
            return _liveEntities.size();
        };

        gpointer get_item_vfunc(guint position) override {
            return (new NBody::Simulation::Particle(_simulation, _liveEntities[position]))->gobj();
        };

    protected:

        NBody::Simulation &_simulation;
        std::vector<NBody::Entity> _liveEntities;

        void add_particles(const std::vector<NBody::Entity> &particles);

    };

}


#endif //N_BODY_PARTICLESLISTMODEL_H
