//
// Created by Jackson Campolattaro on 6/24/22.
//

#ifndef N_BODY_BINDABLE_H
#define N_BODY_BINDABLE_H

#include "NBody/Simulation/Simulation.h"

namespace UI {

    template<typename ...Types>
    class Bindable {
    private:

        sigc::connection _connection;

        std::shared_ptr<NBody::Simulation::Particle> _particle = {};

    public:

        virtual void update(Types &... value) = 0;
        virtual void update() = 0;

        template<typename T>
        void changed(const T &value) {
            assert(_particle);
            _particle->emplace_or_replace<T>(value);
            _particle->get<sigc::signal<void()>>().emit();
        }

        template<typename T, typename ...Args>
        void changed(const T &first, Args... remaining) {
            changed(first);
            changed(remaining...);
        }

        void bind(std::shared_ptr<NBody::Simulation::Particle> &particle) {
            if (_particle) unbind();

            _particle = particle;

            // If the particle doesn't have the types we're trying to bind to, hide it!
            if (!_particle->all_of<Types...>()) {
                update();
                return;
            }

            // If nobody else is already watching this particle, give it a signal
            // Whenever the particle is marked as changed, update the view
            _connection = particle->get_or_emplace<sigc::signal<void()>>().connect([&] {
                assert(_particle); // there should be an associated particle
                update(_particle->get<Types>()...);
            });

            update(particle->get<Types>()...);
        }

        void unbind() {

            // Remove the connection, so this widget no longer updates
            _connection.disconnect();

            // If nobody else is still watching this particle, remove its signal
            if (_particle && _particle->valid() && _particle->get<sigc::signal<void()>>().empty())
                _particle->remove<sigc::signal<void()>>();

            // Clean up -- unbound Bindables aren't associated with any particular particle
            _particle = {};
        };
    };
}

#endif //N_BODY_BINDABLE_H
