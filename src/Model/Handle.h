//
// Created by jackcamp on 9/3/20.
//

#ifndef N_BODY_HANDLE_H
#define N_BODY_HANDLE_H

#include <deque>

namespace Model {

    template<typename T>
    class Handle {
    public:

        Handle(std::deque<T> &vector, size_t index) :
                _vector(vector),
                _index(index) {}

        Handle(std::deque<T> &vector, const T &value) :
                _vector(vector) {

            _vector.push_back(value);
            _index = _vector.size() - 1;
        }

        T &operator*() {
            return _vector[_index];
        }

        const T &operator*() const {
            return _vector[_index];
        }

    private:

        std::deque<T> &_vector;
        size_t _index;

    };
}


#endif //N_BODY_HANDLE_H
