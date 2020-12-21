
find_package(Gtkmm REQUIRED)
find_package(spdlog REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

add_executable(NBody
        main.cpp
        Logger.h Logger.cpp
        Application.h Application.cpp
        )

target_link_libraries(NBody PUBLIC

        Model

        spdlog::spdlog
        ${GTKMM_LIBRARIES}
        ${RapidJSON_LIBRARIES}
        )
