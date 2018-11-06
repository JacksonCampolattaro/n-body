

cmake_minimum_required(VERSION 3.12)
project(n_body_headless)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -fopenmp -march=native")

add_executable(n_body_headless n_body/body.h n_body/body.cpp n_body/simulation.cpp n_body/simulation.h n_body/relationship.cpp n_body/relationship.h n_body/main.cpp n_body/viewport.cpp n_body/viewport.h n_body/drawable.cpp n_body/drawable.h n_body/octant.cpp n_body/octant.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenMP REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES} glfw gtest gtest_main)

