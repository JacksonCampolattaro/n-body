//
// Created by Jackson Campolattaro on 8/3/22.
//

#ifndef N_BODY_PARTICLEIDVIEW_H
#define N_BODY_PARTICLEIDVIEW_H

#include <fmt/format.h>

#include <gtkmm/label.h>
#include <gtkmm/sorter.h>

#include <NBody/Simulation/Simulation.h>

namespace UI {

    class ParticleIDView : public Gtk::Label {
    public:

        ParticleIDView() {
            set_use_markup();
        }

        void bind(std::shared_ptr<NBody::Simulation::Particle> &particle) {
            assert(particle);
            set_label(fmt::format(
                    R"(<span font_desc="monospace">#{}</span>)",
                    (ENTT_ID_TYPE) particle->entity()
            ));
        }

        void unbind() {
            set_label("#?");
        }
    };

}

#endif //N_BODY_PARTICLEIDVIEW_H
