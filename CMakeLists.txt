cmake_minimum_required(VERSION 3.12)
project(n_body_headless)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -fopenmp -march=native")

add_executable(n_body_headless n_body/orbitalBody.h n_body/orbitalBody.cpp n_body/simulationState.cpp n_body/simulationState.h n_body/relationship.cpp n_body/relationship.h n_body/main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES})

