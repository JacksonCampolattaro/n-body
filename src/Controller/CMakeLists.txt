
# Add Gtkmm's include files
find_package(Gtkmm REQUIRED)

# Add spdlog's include files
find_package(spdlog REQUIRED)

# Add rapidjson
find_package(RapidJSON REQUIRED)

add_library(Controller

        Application.h Application.cpp
        Logger.h Logger.cpp
        )

target_include_directories(Controller PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Controller PUBLIC

        Model

        spdlog::spdlog

        ${GTKMM_LIBRARIES}

        #RapidJSON::rapidjson
        ${RapidJSON_LIBRARIES}
        )
