find_package(Gtkmm REQUIRED)

add_library(Interface

        Interface.h
        Viewport.cpp Viewport.h)

target_include_directories(Interface PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Interface PUBLIC

        View

        ${GTKMM_LIBRARIES}
        )
