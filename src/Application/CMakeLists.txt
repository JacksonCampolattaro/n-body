
add_executable(NBody
        main.cpp
        Logger.h Logger.cpp
        Application.h Application.cpp
        Interface/Interface.cpp Interface/Interface.h
        Interface/Simple.cpp Interface/Simple.h
        )

target_link_libraries(NBody PUBLIC

        NBody::Simulation

        spdlog::spdlog
        ${GTKMM_LIBRARIES}
        )
