
# Add necessary libraries
find_package(spdlog REQUIRED)
find_package(Gtkmm REQUIRED)

# Include LibAdwaita
pkg_check_modules(ADWAITA REQUIRED libadwaita-1)
link_directories(${ADWAITA_LIBRARY_DIRS})
include_directories(${ADWAITA_INCLUDE_DIRS})

# Add resource files
find_package(Gio REQUIRED)
find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
set(RESOURCE_DIR ${PROJECT_SOURCE_DIR}/resources)
set(GRESOURCE_C resources.c)
set(GRESOURCE_XML gresource.xml)
set(GRESOURCE_XML_IN ${RESOURCE_DIR}/gresource.xml.in)
set(GRESOURCE_DEPENDENCIES CACHE INTERNAL "GResource dependencies for dummy")
set(OLD ${CMAKE_CURRENT_SOURCE_DIR}/${GRESOURCE_XML_IN})
set(NEW ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_XML})
if (NOT EXISTS ${NEW} OR (${OLD} IS_NEWER_THAN ${NEW}))
    configure_file(${GRESOURCE_XML_IN} ${GRESOURCE_XML})
    execute_process(
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMAND ${GLIB_COMPILE_RESOURCES} --generate-dependencies ${GRESOURCE_XML}
            OUTPUT_VARIABLE GRESOURCE_DEPENDENCIES
    )
    # OUTPUT_VARIABLE is not a list but a single string value with newlines
    # Convert it to a list and pop out the last newline character
    string(REPLACE "\n" ";" GRESOURCE_DEPENDENCIES ${GRESOURCE_DEPENDENCIES})
    list(POP_BACK GRESOURCE_DEPENDENCIES)
    list(TRANSFORM GRESOURCE_DEPENDENCIES PREPEND ${RESOURCE_DIR}/)
endif ()
message(STATUS ${RESOURCE_DIR})
message(STATUS ${GRESOURCE_DEPENDENCIES})
add_custom_command(
        OUTPUT ${GRESOURCE_C}
        WORKING_DIRECTORY ${RESOURCE_DIR}
        COMMAND ${GLIB_COMPILE_RESOURCES}
        ARGS
        --generate-source
        --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
        ${NEW}
        VERBATIM
        MAIN_DEPENDENCY ${GRESOURCE_XML}
        DEPENDS ${GRESOURCE_DEPENDENCIES}
)
set_source_files_properties(
        ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
        PROPERTIES GENERATED TRUE
)

add_executable(NBody_gtkmm4
        main.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C} # fixme this is terrible, there must be a better way
        Application.cpp Application.h
        UI/Simple.cpp UI/Simple.h
        UI/Interactive.cpp UI/Interactive.h
        UI/Sidebar/Sidebar.cpp UI/Sidebar/Sidebar.h UI/Sidebar/Panel.h UI/Sidebar/ParticlesPanel.cpp UI/Sidebar/ParticlesPanel.h UI/Sidebar/PhysicsPanel.cpp UI/Sidebar/PhysicsPanel.h UI/Sidebar/CameraPanel.cpp UI/Sidebar/CameraPanel.h UI/Sidebar/SolverPanel.cpp UI/Sidebar/SolverPanel.h
        UI/Widgets/LabeledWidget.h
        UI/Widgets/ParticlesListView.cpp UI/Widgets/ParticlesListView.h UI/Widgets/ParticleEntry.cpp UI/Widgets/ParticleEntry.h UI/Widgets/VectorView.h UI/Widgets/SphereView.cpp UI/Widgets/SphereView.h UI/Widgets/VectorEditable.cpp UI/Widgets/VectorEditable.h UI/Widgets/BuilderWidget.h UI/Widgets/FloatEntry.cpp UI/Widgets/FloatEntry.h)

target_link_libraries(NBody_gtkmm4 PUBLIC

        NBody::Simulation
        NBody::View

        spdlog::spdlog
        ${GTKMM_LIBRARIES}
        ${ADWAITA_LIBRARIES}
        )
