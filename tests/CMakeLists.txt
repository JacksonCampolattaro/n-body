
cmake_minimum_required(VERSION 3.12)

project(unitTests)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -fopenmp -march=native")

# Adding n_body files to tests compile path
add_executable(unitTests ../src/n_body/interface/Viewport.cpp ../src/n_body/interface/Viewport.h ../src/n_body/interface/Drawable.cpp ../src/n_body/interface/Drawable.h ../src/n_body/model/Octant.cpp ../src/n_body/model/Octant.h ../src/n_body/Tracker.cpp ../src/n_body/Tracker.h ../src/n_body/model/Body.cpp ../src/n_body/model/Body.h ../src/n_body/model/Simulation.cpp ../src/n_body/model/Simulation.h testAll.cpp bodyTest.cpp simulationTest.cpp octantTest.cpp)

# Making sure the right libraries are present
find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
find_package(glfw3 REQUIRED)

# Links graphics and testing libraries
target_link_libraries(unitTests ${OPENGL_LIBRARIES} glfw tbb gtest gtest_main)
