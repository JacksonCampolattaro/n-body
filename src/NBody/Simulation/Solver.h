//
// Created by Jackson Campolattaro on 19/04/2022.
//

#ifndef N_BODY_SOLVER_H
#define N_BODY_SOLVER_H

#include <sigc++/sigc++.h>

#include <spdlog/spdlog.h>

#include <tbb/global_control.h>
#include <tbb/parallel_for_each.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Physics/Rule.h>
#include <NBody/Simulation/TypedDispatcher.h>

namespace NBody {

    using namespace Physics;

    typedef std::array<char, 64> Status;

    class Solver : public Glib::Object {
    protected:

        inline static float _dt = 0.001;

        Simulation &_simulation;
        Physics::Rule &_rule;
        std::size_t _maxThreadCount = tbb::global_control::active_value(tbb::global_control::max_allowed_parallelism);

        mutable sigc::signal<void()> _signal_finished;
        sigc::slot<void()> _slot_step;

        TypedDispatcher<std::chrono::duration<double>> _computeTimeDispatcher;
        TypedDispatcher<Status> _statusDispatcher;

        std::shared_ptr<std::thread> _thread;
        Glib::Dispatcher _dispatcher;

    public:


        Solver(Simulation &simulation, Physics::Rule &rule) :
                Glib::ObjectBase(typeid(NBody::Solver)),
                Glib::Object(),
                _simulation(simulation), _rule(rule) {

            _slot_step = sigc::mem_fun(*this, &Solver::on_step);

            // When the underlying solver announces that it's complete...
            _dispatcher.connect([&] {

                spdlog::trace("Joining completed solver");

                // Join the solver thread
                assert(_thread);
                _thread->join();
                _thread.reset();

                // Announce the completion to the rest of the UI
                _simulation.view<sigc::signal<void()>>().each([](auto &s) { s.emit(); });
                _simulation.signal_changed.emit();
                _statusDispatcher.emit({"Stopped"});
                signal_finished().emit();
            });
        };

        ~Solver() override {
            if (_thread)
                _thread->join();
            _thread.reset();
        }

        virtual std::string id() = 0;

        virtual std::string name() = 0;

        Simulation &simulation() { return _simulation; }

        Physics::Rule &rule() { return _rule; }

        std::size_t &maxThreadCount() { return _maxThreadCount; }

        const std::size_t &maxThreadCount() const { return _maxThreadCount; }

        static float &timeStep() { return Solver::_dt; }

        void step() {

            // Update accelerations, based on positions
            updateAccelerations();

            // Update velocities, based on acceleration
            updateVelocities();

            // Update positions, based on velocity
            updatePositions();
        }

        virtual void updateAccelerations() = 0;

        void updateVelocities() {
            _statusDispatcher.emit({"Updating velocities"});
            auto view = _simulation.template view<const Acceleration, Velocity>();
            tbb::parallel_for_each(view, [&](Entity e) {
                const auto &a = view.template get<const Acceleration>(e);
                auto &v = view.template get<Velocity>(e);
                v = v + (a * _dt);
            });
        }

        void updatePositions() {
            // While the solver is modifying simulation values, the simulation should be locked for other threads
            std::scoped_lock l(_simulation.mutex);

            _statusDispatcher.emit({"Updating positions"});
            auto view = _simulation.template view<const Velocity, Position>();

            tbb::parallel_for_each(view, [&](Entity e) {
                const auto &v = view.template get<const Velocity>(e);
                auto &p = view.template get<Position>(e);
                p = p + (v * _dt);
            });
        }

    public:

        sigc::signal<void()> &signal_finished() const { return _signal_finished; };

        sigc::signal<void(std::chrono::duration<double>)> &
        signal_computeTime() { return _computeTimeDispatcher.signal(); };

        virtual sigc::signal<void(Status)> &
        signal_status() { return _statusDispatcher.signal(); };

        sigc::slot<void()> &slot_step() { return _slot_step; };

    private:

        void on_step() {

            // This should be idempotent, calling more than once should only spawn one background thread
            if (_thread) {
                spdlog::trace("Attempted to spawn multiple background threads");
                return;
            }

            spdlog::trace("Launching solver in new thread");

            // Spawn a new thread when the user requests a step
            _thread = std::make_shared<std::thread>([&] {

                spdlog::trace("Beginning simulation step");
                _statusDispatcher.emit({"Starting step"});

                // Cap the number of threads based on the user's preference
                tbb::global_control c{tbb::global_control::max_allowed_parallelism, _maxThreadCount};

                auto startTime = std::chrono::steady_clock::now();
                step();
                auto finishTime = std::chrono::steady_clock::now();
                std::chrono::duration<double> duration = finishTime - startTime;

                _statusDispatcher.emit({"Finished step"});
                _computeTimeDispatcher.emit(duration);
                spdlog::trace("Finished simulation step in {} s", duration.count());

                // Notify the main thread that we're finished (so we can join)
                _dispatcher.emit();
            });
        }
    };

}

#endif //N_BODY_SOLVER_H
