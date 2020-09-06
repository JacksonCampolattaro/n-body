#include <cassert>

#include "../src/Model/Physics/ActiveElement.h"

using Model::Physics::ActiveElement;

int main() {

    std::stringstream stream;

    // Save the original to a stream
    ActiveElement original(100);
    stream << original;
    std::cout << original;

    // Load a copy from the stream
    ActiveElement copy(1);
    stream >> copy;
    std::cout << copy;

    // Make sure the two match
    // TODO

    return 0;
}
