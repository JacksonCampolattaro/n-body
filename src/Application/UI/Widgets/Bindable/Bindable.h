//
// Created by Jackson Campolattaro on 6/24/22.
//

#ifndef N_BODY_BINDABLE_H
#define N_BODY_BINDABLE_H

#include <NBody/Simulation/Simulation.h>

namespace UI {

    template<typename T>
    class Bindable {
    private:

        sigc::connection _connection;

        std::shared_ptr<NBody::Simulation::Particle> _particle = {};

    public:

        virtual void update(T &value) = 0;

        void changed(const T &value) {
            assert(_particle);
            _particle->emplace_or_replace<T>(value);
        }

        void bind(std::shared_ptr<NBody::Simulation::Particle> &particle) {
            if (_particle) unbind();

            _particle = particle;

            // If nobody else is already watching this particle, give it a signal
            // Whenever the particle is marked as changed, update the view
            _connection = particle->get_or_emplace<sigc::signal<void()>>().connect([&] {
                assert(_particle); // there should be an associated particle
                update(_particle->get<T>());
            });

            update(particle->get<T>());
        }

        void unbind() {

            // Remove the connection, so this widget no longer updates
            _connection.disconnect();

            // If nobody else is still watching this particle, remove its signal
            if (_particle->get<sigc::signal<void()>>().empty())
                _particle->remove<sigc::signal<void()>>();

            // Clean up -- unbound Bindables aren't associated with any particular particle
            _particle = {};
        };
    };
}

#endif //N_BODY_BINDABLE_H
