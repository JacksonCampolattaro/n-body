
include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.9.1)
FetchContent_MakeAvailable(json)

FetchContent_Declare(entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.5.2)
FetchContent_MakeAvailable(entt)

add_library(Simulation
        Simulation.h Simulation.cpp
        )

target_include_directories(Simulation PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Simulation PUBLIC

        NBody::Physics
        NBody::Graphics

        EnTT::EnTT
        nlohmann_json::nlohmann_json
        )

add_library(NBody::Simulation ALIAS Simulation)