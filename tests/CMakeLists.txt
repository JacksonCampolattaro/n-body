
cmake_minimum_required(VERSION 3.12)

project(unitTests)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -fopenmp -march=native")

# Adding n_body files to tests compile path
add_executable(unitTests ../src/n_body/view/Drawable.cpp ../src/n_body/view/Drawable.h ../src/n_body/model/calculation/BarnesHut/Octant.cpp ../src/n_body/model/calculation/BarnesHut/Octant.h ../src/n_body/model/Body.cpp ../src/n_body/model/Body.h testAll.cpp bodyTest.cpp simulationTest.cpp octantTest.cpp ../src/n_body/model/PhysicsContext.cpp ../src/n_body/model/PhysicsContext.h ../src/n_body/model/calculation/BarnesHut/BarnesHutSolver.cpp ../src/n_body/model/calculation/BarnesHut/BarnesHutSolver.h ../src/n_body/model/Model.cpp ../src/n_body/model/Model.h ../src/n_body/view/View.cpp ../src/n_body/view/View.h ../src/n_body/view/GLToolkit.cpp ../src/n_body/view/GLToolkit.h ../src/n_body/view/Observer.cpp ../src/n_body/view/Observer.h ../src/n_body/view/ViewPort.cpp ../src/n_body/view/ViewPort.h)

# Making sure the right libraries are present
find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
find_package(glfw3 REQUIRED)

# Links graphics and testing libraries
target_link_libraries(unitTests ${OPENGL_LIBRARIES} glfw tbb gtest gtest_main)
