
find_package(Magnum REQUIRED
        GL
        )

add_executable(NBody
        main.cpp
        Logger.h Logger.cpp
        Application.h Application.cpp
        Interface/Interface.cpp Interface/Interface.h
        Interface/Simple.cpp Interface/Simple.h
        Interface/Interactive.cpp Interface/Interactive.h Interface/ConfigurationPanel/ConfigurationPanel.cpp Interface/ConfigurationPanel/ConfigurationPanel.h)

target_link_libraries(NBody PUBLIC

        NBody::Simulation
        NBody::View

        spdlog::spdlog
        ${GTKMM_LIBRARIES}
        )
