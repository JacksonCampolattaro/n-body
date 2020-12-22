
# Add openmp
find_package(OpenMP REQUIRED)

# Add spdlog's include files
find_package(spdlog REQUIRED)

# Add rapidjson
find_package(RapidJSON REQUIRED)

# Add rapidjson
find_package(Gtkmm REQUIRED)

add_library(Model

        Entity.h Entity.cpp
        Position.h
        Velocity.h
        Simulation.h Simulation.cpp

        Solver/Solver.h

        Physics/Physics.h
        Physics/Rule.h Physics/Rule.cpp
        Physics/ActiveElement.h Physics/ActiveElement.cpp
        Physics/PassiveElement.h Physics/PassiveElement.cpp

        Drawable/Color.h
        Drawable/Drawable.h Drawable/Drawable.cpp
        ../Interface/Interface.h)

target_include_directories(Model PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Model PUBLIC

        spdlog::spdlog

        OpenMP::OpenMP_CXX

        ${GTKMM_LIBRARIES}

        #RapidJSON::rapidjson
        ${RapidJSON_LIBRARIES}
        )

add_library(NBody::Model ALIAS Model)