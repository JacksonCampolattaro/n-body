#include <cassert>

#include "../src/Model/Physics/PassiveElement.h"

using Model::Physics::PassiveElement;

int main() {

    std::stringstream stream;

    // Save the original to a stream
    PassiveElement original(100);
    stream << original;
    std::cout << original;

    // Load a copy from the stream
    PassiveElement copy(1);
    stream >> copy;
    std::cout << copy;

    // Make sure the two match
    // TODO

    return 0;
}
