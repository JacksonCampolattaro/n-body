
#add_executable(bodies
#        bodies.cpp)
#
#target_link_libraries(bodies
#        NBody::Simulation)


find_package(Catch2 QUIET)
if (NOT Catch2_FOUND)

    include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v2.13.1)
    FetchContent_MakeAvailable(Catch2)

endif ()

add_executable(test main.cpp
        testSimulation.cpp
        )

target_link_libraries(test LINK_PUBLIC
        NBody::Simulation
        Catch2::Catch2)
