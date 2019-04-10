
# Sets CMake standard
set(CMAKE_CXX_STANDARD 14)

# Adds my files to compilation path
add_executable(${CMAKE_PROJECT_NAME} n_body/main.cpp n_body/interface/Drawable.cpp n_body/interface/Drawable.h n_body/Tracker.cpp n_body/Tracker.h n_body/model/calculation/BarnesHut/Octant.cpp n_body/model/calculation/BarnesHut/Octant.h n_body/model/Body.cpp n_body/model/Body.h n_body/model/Simulation.cpp n_body/model/Simulation.h n_body/interface/Viewport.cpp n_body/interface/Viewport.h n_body/model/calculation/Solver.cpp n_body/model/calculation/Solver.h n_body/model/calculation/Naive/NaiveSolver.cpp n_body/model/calculation/Naive/NaiveSolver.h n_body/model/PhysicsContext.cpp n_body/model/PhysicsContext.h)

# Confirms that OpenMP is present and installed properly
find_package(OpenMP REQUIRED)

# Sets OpenMP compilation flags to allow for multithreaded performance -mavx allows avx instructions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx ${OpenMP_CXX_FLAGS}")

# Makes sure OpenGL libraries are present and installed properly
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Addls OpenGL and tbb libraries to compilation path
target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES} glfw tbb)

