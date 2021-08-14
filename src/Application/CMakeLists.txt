
find_package(Magnum REQUIRED
        GL
        )

find_package(Gio REQUIRED)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

set(RESOURCE_DIR     ${PROJECT_SOURCE_DIR}/resources)
set(GRESOURCE_C      resources.c)
set(GRESOURCE_XML    gresource.xml)
set(GRESOURCE_XML_IN ${RESOURCE_DIR}/gresource.xml.in)
set(GRESOURCE_DEPENDENCIES CACHE INTERNAL "GResource dependencies for dummy")

set(OLD ${CMAKE_CURRENT_SOURCE_DIR}/${GRESOURCE_XML_IN})
set(NEW ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_XML})

if(NOT EXISTS ${NEW} OR (${OLD} IS_NEWER_THAN ${NEW}))
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
endif()

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
        Logger.h Logger.cpp
        Application.h Application.cpp
        Interface/Interface.cpp Interface/Interface.h
        Interface/Simple.cpp Interface/Simple.h
        Interface/Interactive.cpp Interface/Interactive.h
        Interface/ConfigurationPanel/ConfigurationPanel.cpp Interface/ConfigurationPanel/ConfigurationPanel.h
        Interface/ConfigurationPanel/LabeledWidget.h
        Interface/ConfigurationPanel/Panes/BodiesPane.cpp Interface/ConfigurationPanel/Panes/BodiesPane.h
        Interface/ConfigurationPanel/Panes/Pane.cpp Interface/ConfigurationPanel/Panes/Pane.h)

target_link_libraries(NBody PUBLIC

        NBody::Simulation
        NBody::View

        spdlog::spdlog
        ${GTKMM_LIBRARIES}
        )
