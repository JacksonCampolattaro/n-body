
# Set C++ version
set(CMAKE_CXX_STANDARD 14)

# Add Gtkmm's include files
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# Set files to be compiled
add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        View/Application.cpp View/Application.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        ${GTKMM_LIBRARIES}
        )
