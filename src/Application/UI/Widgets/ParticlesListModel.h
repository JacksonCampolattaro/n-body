//
// Created by Jackson Campolattaro on 6/23/22.
//

#ifndef N_BODY_PARTICLESLISTMODEL_H
#define N_BODY_PARTICLESLISTMODEL_H

#include <giomm/listmodel.h>

#include <NBody/Simulation/Simulation.h>

namespace UI {

    class ParticlesListModel : public Gio::ListModel, public Glib::Object {
    public:

        explicit ParticlesListModel(NBody::Simulation &simulation)
                : Glib::ObjectBase(typeid(ParticlesListModel)), Glib::Object(), _simulation(simulation) {}

        static Glib::RefPtr<ParticlesListModel> create(NBody::Simulation &simulation) {
            return Glib::make_refptr_for_instance(new ParticlesListModel(simulation));
        }

        GType get_item_type_vfunc() override {
            return NBody::Simulation::Particle::get_type();
        };

        guint get_n_items_vfunc() override {
            return _simulation.size();
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
