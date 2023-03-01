
#include(FetchContent)
#FetchContent_Declare(json
#        GIT_REPOSITORY https://github.com/nlohmann/json.git
#        GIT_TAG v3.9.1)
#FetchContent_MakeAvailable(json)

find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)

add_library(Physics
        Position.h
        Velocity.h
        Mass.cpp Mass.h
        BoundingBox.h
        SummaryType.h
        Summaries/CenterOfMassSummary.h
        Summaries/QuadrupoleMassSummary.h
        Summaries/AccelerationSummary.h
        Summaries/DualSummary.h
        Summaries/BoundingBoxSummary.h
        Rule.h Force.h Acceleration.h Quadrupole.h Summaries/QuadrupoleDualSummary.h QuadrupoleAcceleration.h Summaries/QuadrupoleAccelerationSummary.h SymmetricTensor3.h Multipole.h Summaries/MultipoleMassSummary.h)

target_include_directories(Physics PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Physics PUBLIC
        nlohmann_json::nlohmann_json)

add_library(NBody::Physics ALIAS Physics)

add_subdirectory(test)
