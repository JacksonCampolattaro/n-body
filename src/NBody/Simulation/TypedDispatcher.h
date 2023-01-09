//
// Created by Jackson Campolattaro on 7/9/22.
//

#ifndef N_BODY_TYPEDDISPATCHER_H
#define N_BODY_TYPEDDISPATCHER_H

#include <spdlog/spdlog.h>
#include <glibmm/dispatcher.h>
#include <boost/lockfree/queue.hpp>

namespace NBody {

    template<typename T>
    class TypedDispatcher {
    private:

        Glib::Dispatcher _dispatcher;

        sigc::signal<void(T)> _signal;

        boost::lockfree::queue<T> _queue{128};

    public:

        TypedDispatcher() {

            // Whenever the dispatcher is triggered, data may have been pushed to the queue
            _dispatcher.connect([this]() {

                // Emit signals until the queue has been fully consumed
                while (!_queue.empty()) {
                    T value;
                    _queue.pop(value);
                    _signal.emit(value);
                }
            });
        }

        sigc::signal<void(T)> &signal() { return _signal; };

        void emit(const T &value) {
            // There's no point in emitting a value if the signal isn't connected to anything
            if (signal().empty()) return;

            _queue.push(value);
            _dispatcher.emit();
        }
    };

}

#endif //N_BODY_TYPEDDISPATCHER_H
