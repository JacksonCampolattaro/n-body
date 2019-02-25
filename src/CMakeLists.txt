
# Sets CMake standard
set(CMAKE_CXX_STANDARD 14)

# Adds my files to compilation path
add_executable(${CMAKE_PROJECT_NAME} n_body/body.h n_body/body.cpp n_body/simulation.cpp n_body/simulation.h n_body/relationship.cpp n_body/relationship.h n_body/main.cpp n_body/viewport.cpp n_body/viewport.h n_body/drawable.cpp n_body/drawable.h n_body/tracker.cpp n_body/tracker.h n_body/octant.cpp n_body/octant.h)

# Confirms that OpenMP is present and installed properly
find_package(OpenMP REQUIRED)

# Sets OpenMP compilation flags to allow for multithreaded performance -mavx allows avx instructions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx ${OpenMP_CXX_FLAGS}")

# Makes sure OpenGL libraries are present and installed properly
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Addls OpenGL and tbb libraries to compilation path
target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES} glfw tbb)

