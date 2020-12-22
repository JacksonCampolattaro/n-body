include(FetchContent)
include(ExternalProject)

find_package(OpenMP REQUIRED)
find_package(spdlog REQUIRED)
find_package(Gtkmm REQUIRED)

add_subdirectory(Application)
add_subdirectory(NBody)

