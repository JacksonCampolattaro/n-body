//
// Created by Jackson Campolattaro on 1/11/23.
//

#ifndef N_BODY_FIELDTREE_H
#define N_BODY_FIELDTREE_H

#include <sigc++/sigc++.h>

#include <span>
#include <numeric>

#include "NBody/Physics/Rule.h"

#include "NBody/Simulation/Simulation.h"

namespace NBody {

    using namespace Physics;

    template<typename NodeImplementation>
    class FieldTreeBase {
    public:

        using Node = NodeImplementation;

    private:

        Simulation &_simulation;
        std::vector<Entity> _indices;

        Node _root;

        Glib::Dispatcher _dispatcher;

    public:

        mutable sigc::signal<void()> signal_changed;
        mutable std::mutex mutex;

        TreeBase(Simulation &simulation) :
                _simulation(simulation),
                _indices{relevantEntities()},
                _root{std::span<Entity>{_indices}} {

            // If the simulation has new particles, add them to the list
            _simulation.signal_particles_added.connect([&](auto newEntities) {
                _indices = relevantEntities();
                _root = NodeImplementation{_indices};
            });

            // If the simulation has particles removed, take them from the list
            _simulation.signal_particles_removed.connect([&](auto removedEntities) {
                _indices = relevantEntities();
                _root = NodeImplementation{_indices};
            });

            _dispatcher.connect(signal_changed.make_slot());
        }

        void refine() {
            std::scoped_lock lock{mutex};

            build();

            // Notify the UI of the change
            _dispatcher.emit();
        }

        virtual void build() = 0;

        std::vector<Entity> relevantEntities() {
            return {_simulation.group<const Position, const Mass>(entt::get<ActiveTag>).begin(),
                    _simulation.group<const Position, const Mass>(entt::get<ActiveTag>).end()};
        }

        [[nodiscard]] const Simulation &simulation() const { return _simulation; }

        [[nodiscard]]  Simulation &simulation() { return _simulation; }

        [[nodiscard]] std::vector<Entity> &indices() { return _indices; }

        [[nodiscard]] const std::vector<Entity> &indices() const { return _indices; }

        [[nodiscard]] const NodeImplementation &root() const { return _root; }

        [[nodiscard]] NodeImplementation &root() { return _root; }

    };
}


#endif //N_BODY_FIELDTREE_H
