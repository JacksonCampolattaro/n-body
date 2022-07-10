
find_package(PkgConfig)
pkg_check_modules(GIO gio-2.0)
link_directories(${GIO_LIBRARY_DIRS})
include_directories(${GIO_INCLUDE_DIRS})
