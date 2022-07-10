
find_package(Catch2 REQUIRED)

add_executable(test_Graphics main.cpp
        color.cpp sphere.cpp)

target_link_libraries(test_Graphics PRIVATE
        NBody::Graphics
        Catch2::Catch2)

add_test(NAME TestGraphics COMMAND test_Graphics)
