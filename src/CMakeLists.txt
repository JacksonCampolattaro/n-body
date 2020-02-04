
# Set C++ version
set(CMAKE_CXX_STANDARD 17)

# Add Gtkmm's include files
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# Set files to be compiled
add_executable(${CMAKE_PROJECT_NAME}
        main.cpp

        Controller/Controller.h
    Controller/Logger.cpp
        Controller/Logger.h Controller/Application.cpp Controller/Application.h View/Builder.cpp View/Builder.h View/MagnumViewport.cpp View/MagnumViewport.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        ${GTKMM_LIBRARIES}
        )
