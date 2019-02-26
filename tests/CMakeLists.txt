
cmake_minimum_required(VERSION 3.12)

project(unitTests)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -fopenmp -march=native")

# Adding n_body files to tests compile path
add_executable(unitTests ../src/n_body/body.h ../src/n_body/body.cpp ../src/n_body/simulation.cpp ../src/n_body/simulation.h ../src/n_body/viewport.cpp ../src/n_body/viewport.h ../src/n_body/model/drawable.cpp ../src/n_body/model/drawable.h ../src/n_body/model/octant.cpp ../src/n_body/model/octant.h ../src/n_body/tracker.cpp ../src/n_body/tracker.h testAll.cpp bodyTest.cpp simulationTest.cpp threadSafe_octantTest.cpp)

# Making sure the right libraries are present
find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
find_package(glfw3 REQUIRED)

# Links graphics and testing libraries
target_link_libraries(unitTests ${OPENGL_LIBRARIES} glfw tbb gtest gtest_main)
