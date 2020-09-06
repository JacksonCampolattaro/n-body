
#include "../src/Model/Position.h"

#include <cassert>
#include <iostream>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using Model::Position;

int main() {

    rapidjson::StringBuffer buffer;

    // Save the original to a stream
    Position original(1, 2, 3);
    std::cout << "original: " << original << std::endl;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer << original;

    std::string saved = buffer.GetString();
    std::cout << "archive: " << saved << std::endl;

    // Load a copy from the stream
    Position copy(0, 0, 0);
    rapidjson::Document document;
    document.Parse(saved.c_str());
    document >> copy;
    std::cout << "copy: " << copy << std::endl;

    // Make sure the two match
    assert(original == copy);

    return 0;
}