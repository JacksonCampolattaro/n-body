
# Add Gtkmm's include files
find_package(Gtkmm REQUIRED)

add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)

# Set files to be compiled
add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        )

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        Model
        View
        Controller
        )
