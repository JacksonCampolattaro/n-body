
include(FetchContent)
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.9.1)
FetchContent_MakeAvailable(json)

add_library(Physics
        Position.h Position.cpp
        Velocity.h Velocity.cpp
        ActiveMass.cpp ActiveMass.h)

target_include_directories(Physics PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Physics PUBLIC
        nlohmann_json::nlohmann_json
        )

add_library(NBody::Physics ALIAS Physics)
