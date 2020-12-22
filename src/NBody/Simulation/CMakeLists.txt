include(FetchContent)
FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(entt)

add_library(Simulation

        Simulation.h Simulation.cpp
        )

target_include_directories(Simulation PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Simulation PUBLIC
        EnTT::EnTT
        )
