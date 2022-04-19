find_package(PkgConfig)
pkg_check_modules(SIGCPP sigc++-3.0)
link_directories(${SIGCPP_LIBRARY_DIRS})
include_directories(${SIGCPP_INCLUDE_DIRS})
