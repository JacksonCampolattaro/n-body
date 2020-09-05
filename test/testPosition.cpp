#include <cassert>

#include "../src/Model/Position.h"

using Model::Position;

int main() {

    std::stringstream stream;

    // Save the original to a stream
    Position original(1, 2, 3);
    stream << original;

    // Load a copy from the stream
    Position copy(0, 0, 0);
    stream >> copy;

    // Make sure the two match
    assert(original == copy);

    return 0;
}