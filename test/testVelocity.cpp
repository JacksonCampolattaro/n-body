
#include "../src/Model/Velocity.h"

#include <cassert>
#include <iostream>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using Model::Velocity;

int main() {

    rapidjson::StringBuffer buffer;

    // Save the original to a stream
    Velocity original(1, 2, 3);
    std::cout << "original: " << original << std::endl;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer << original;

    std::string saved = buffer.GetString();
    std::cout << "archive: " << saved << std::endl;

    // Load a copy from the stream
    Velocity copy(0, 0, 0);
    rapidjson::Document document;
    document.Parse(saved.c_str());
    document >> copy;
    std::cout << "copy: " << copy << std::endl;

    // Make sure the two match
    assert(original == copy);

    return 0;
}
