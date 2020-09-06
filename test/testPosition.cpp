
#include "../src/Model/Position.h"

#include <cassert>
#include <iostream>

#include <rapidjson/stringbuffer.h>

using Model::Position;

int main() {

    rapidjson::StringBuffer buffer;

    // Save the original to a stream
    Position original(1, 2, 3);
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer << original;

    std::cout << buffer.GetString();

    // Load a copy from the stream
    Position copy(0, 0, 0);

    // Make sure the two match
//    assert(original == copy);

    return 0;
}