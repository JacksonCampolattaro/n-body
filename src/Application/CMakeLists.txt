
add_executable(NBody
        main.cpp
        Logger.h Logger.cpp
        Application.h Application.cpp
        )

target_link_libraries(NBody PUBLIC

        NBody::Simulation
        NBody::View
        NBody::Interface

        spdlog::spdlog
        ${GTKMM_LIBRARIES}
        )
