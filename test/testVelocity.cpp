#include <cassert>

#include "../src/Model/Velocity.h"

using Model::Velocity;

int main() {

    std::stringstream stream;

    // Save the original to a stream
    Velocity original(1, 2, 3);
    stream << original;
    std::cout << original;

    // Load a copy from the stream
    Velocity copy(0, 0, 0);
    stream >> copy;
    std::cout << copy;

    // Make sure the two match
    assert(original == copy);

    return 0;
}
