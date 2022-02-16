
find_package(spdlog REQUIRED)
find_package(Gtkmm REQUIRED)

add_executable(NBody_gtkmm4
        main.cpp Application.cpp Application.h UI/Simple.cpp UI/Simple.h UI/Interactive.cpp UI/Interactive.h)

target_link_libraries(NBody_gtkmm4 PUBLIC

        NBody::Simulation
        NBody::View

        spdlog::spdlog
        ${GTKMM_LIBRARIES}
        )
