
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

add_executable(NBody
        main.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C} # fixme this is terrible, there must be a better way
        Application.cpp Application.h
        UI/Simple.cpp UI/Simple.h
        UI/Interactive.cpp UI/Interactive.h
        UI/Sidebar/Sidebar.cpp UI/Sidebar/Sidebar.h
        UI/Sidebar/Panel.h UI/Sidebar/ParticlesPanel.cpp UI/Sidebar/ParticlesPanel.h UI/Sidebar/PhysicsPanel.cpp UI/Sidebar/PhysicsPanel.h UI/Sidebar/CameraPanel.cpp UI/Sidebar/CameraPanel.h UI/Sidebar/SolverPanel.cpp UI/Sidebar/SolverPanel.h
        UI/Widgets/LabeledWidget.h
        UI/Widgets/Entry/ParticleEntry.cpp UI/Widgets/Entry/ParticleEntry.h
        UI/Widgets/View/VectorView.h
        UI/Widgets/Entry/VectorEntry.cpp UI/Widgets/Entry/VectorEntry.h
        UI/Widgets/BuilderWidget.h
        UI/Widgets/Entry/FloatEntry.cpp UI/Widgets/Entry/FloatEntry.h
        UI/Sidebar/RunPanel.cpp UI/Sidebar/RunPanel.h
        UI/Widgets/ParticlesColumnView.cpp UI/Widgets/ParticlesColumnView.h
        UI/Widgets/View/ParticleMassView.h
        UI/Widgets/BindableListItemFactory.h UI/Widgets/View/ParticlePositionView.h UI/Widgets/View/ParticleVelocityView.h UI/Widgets/Bindable.h UI/Widgets/View/FloatView.h UI/Widgets/Entry/ParticlePositionEntry.h UI/Widgets/Entry/ParticleVelocityEntry.h UI/Widgets/Entry/ParticleMassEntry.h UI/Widgets/Entry/ParticleColorEntry.h UI/Widgets/Entry/ParticleRadiusEntry.h UI/Widgets/View/ParticleColorView.h UI/Widgets/View/ParticleRadiusView.h UI/Widgets/View/ParticleIconView.h UI/Widgets/SaveSimulationDialog.cpp UI/Widgets/SaveSimulationDialog.h UI/Widgets/LoadSimulationDialog.cpp UI/Widgets/LoadSimulationDialog.h UI/Sidebar/NaiveSolverSettings.cpp UI/Sidebar/NaiveSolverSettings.h UI/Sidebar/BarnesHutSolverSettings.cpp UI/Sidebar/BarnesHutSolverSettings.h UI/Widgets/View/TimeView.h)

target_link_libraries(NBody PUBLIC

        NBody::Simulation
        NBody::View

        spdlog::spdlog
        ${GTKMM_LIBRARIES}
        ${ADWAITA_LIBRARIES}
        )
