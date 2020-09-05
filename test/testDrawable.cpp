#include <cassert>

#include "../src/Model/Drawable/Drawable.h"

using Model::Drawable::Drawable;

int main() {

    std::stringstream stream;

    // Save the original to a stream
    Drawable original({0.1, 0.2, 0.3}, 100);
    stream << original;
    std::cout << original;

    // Load a copy from the stream
    Drawable copy({0, 0, 0}, 1);
    stream >> copy;
    std::cout << copy;

    // Make sure the two match

    return 0;
}
