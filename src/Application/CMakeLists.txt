
# Add necessary libraries
find_package(spdlog REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(gtkmm REQUIRED IMPORTED_TARGET gtkmm-4.0 glibmm-2.68 giomm-2.68)
pkg_check_modules(adwaita REQUIRED IMPORTED_TARGET libadwaita-1)

# Add resource files
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
        UI/Sidebar/ParticlesPanel.cpp UI/Sidebar/ParticlesPanel.h
        UI/Sidebar/PhysicsPanel.cpp UI/Sidebar/PhysicsPanel.h
        UI/Sidebar/CameraPanel.cpp UI/Sidebar/CameraPanel.h
        UI/Sidebar/SolverPanel.cpp UI/Sidebar/SolverPanel.h
        UI/Sidebar/RunPanel.cpp UI/Sidebar/RunPanel.h
        UI/ParticlesListModel.cpp UI/ParticlesListModel.h
        UI/Widgets/Entry/ParticleEntry.cpp UI/Widgets/Entry/ParticleEntry.h
        UI/Widgets/View/VectorView.h
        UI/Widgets/Entry/VectorEntry.cpp UI/Widgets/Entry/VectorEntry.h
        UI/Widgets/BuilderWidget.h
        UI/Widgets/Entry/FloatEntry.cpp UI/Widgets/Entry/FloatEntry.h
        UI/Widgets/ListView/ParticlesColumnView.cpp UI/Widgets/ListView/ParticlesColumnView.h
        UI/Widgets/View/ParticleMassView.h
        UI/BindableListItemFactory.h
        UI/Widgets/View/ParticlePositionView.h
        UI/Widgets/View/ParticleVelocityView.h
        UI/Widgets/Bindable.h
        UI/Widgets/View/FloatView.h
        UI/Widgets/Entry/ParticlePositionEntry.h
        UI/Widgets/Entry/ParticleVelocityEntry.h
        UI/Widgets/Entry/ParticleMassEntry.h
        UI/Widgets/Entry/ParticleColorEntry.h
        UI/Widgets/Entry/ParticleRadiusEntry.h
        UI/Widgets/View/ParticleColorView.h
        UI/Widgets/View/ParticleRadiusView.h
        UI/Widgets/View/ParticleIconView.h
        UI/Windows/ParticleEditorWindow.cpp UI/Windows/ParticleEditorWindow.h
        UI/Windows/ParticleListWindow.cpp UI/Windows/ParticleListWindow.h
        UI/Sidebar/NaiveSolverSettings.cpp UI/Sidebar/NaiveSolverSettings.h
        UI/Sidebar/BarnesHutSolverSettings.cpp UI/Sidebar/BarnesHutSolverSettings.h
        UI/Widgets/View/TimeView.h
        UI/Widgets/Entry/ParticleTagEntry.h
        UI/Widgets/View/ParticleTagView.h
        UI/Widgets/View/ParticleIDView.h
        UI/Widgets/Entry/IntEntry.h
        UI/Windows/ParticleGridCreatorWindow.cpp UI/Windows/ParticleGridCreatorWindow.h
        UI/Widgets/ParticleGridCreator.cpp UI/Widgets/ParticleGridCreator.h
        UI/Widgets/VideoRecorder.cpp UI/Widgets/VideoRecorder.h
        UI/Windows/VideoFileChooserDialog.cpp UI/Windows/VideoFileChooserDialog.h
        UI/Windows/ImageFileChooserDialog.cpp UI/Windows/ImageFileChooserDialog.h
        UI/SimulationFileManager.cpp UI/SimulationFileManager.h
        UI/Windows/SimulationFileSaverDialog.cpp UI/Windows/SimulationFileSaverDialog.h
        UI/Windows/SimulationFileLoaderDialog.cpp UI/Windows/SimulationFileLoaderDialog.h
        UI/Widgets/ListView/StackView.cpp UI/Widgets/ListView/StackView.h UI/Widgets/ListView/DropDownView.cpp UI/Widgets/ListView/DropDownView.h UI/Sidebar/ContinuousRunnerController.cpp UI/Sidebar/ContinuousRunnerController.h UI/Sidebar/OneStepRunnerController.cpp UI/Sidebar/OneStepRunnerController.h UI/Sidebar/NStepsRunnerController.cpp UI/Sidebar/NStepsRunnerController.h UI/Sidebar/LinearBVHSolverSettings.cpp UI/Sidebar/LinearBVHSolverSettings.h)

target_link_libraries(NBody PUBLIC

        NBody::Simulation
        NBody::View

        spdlog::spdlog
        PkgConfig::gtkmm
        PkgConfig::adwaita
        )
