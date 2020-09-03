//
// Created by jackcamp on 9/3/20.
//

#ifndef N_BODY_HANDLE_H
#define N_BODY_HANDLE_H

#include <vector>

namespace Model {

    template<typename T>
    class Handle {

        Handle(std::vector<T> &vector, size_t index) :
                _vector(vector),
                _index(index) {}

        Handle(std::vector<T> &vector, const T &value) :
                _vector(vector) {

            _vector.push_back(value);
            _index = _vector.size() - 1;
        }

    private:

        std::vector<T> &_vector;
        size_t _index;

    };
}


#endif //N_BODY_HANDLE_H
