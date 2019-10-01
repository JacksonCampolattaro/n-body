
# Sets CMake standard
set(CMAKE_CXX_STANDARD 14)

# Confirms that OpenMP is present and installed properly
find_package(OpenMP REQUIRED)
#find_package(raylib 2.0 QUIET)

# Confirms that controller libraries are present and installed properly
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# Adds my files to compilation path
add_executable(${CMAKE_PROJECT_NAME} n_body/main.cpp n_body/view/Drawable.cpp n_body/view/Drawable.h n_body/model/calculation/BarnesHut/Octant.cpp n_body/model/calculation/BarnesHut/Octant.h n_body/model/Body.cpp n_body/model/Body.h n_body/model/calculation/Solver.cpp n_body/model/calculation/Solver.h n_body/model/calculation/Naive/NaiveSolver.cpp n_body/model/calculation/Naive/NaiveSolver.h n_body/model/PhysicsContext.cpp n_body/model/PhysicsContext.h n_body/view/View.cpp n_body/view/View.h n_body/model/calculation/BarnesHut/BarnesHutSolver.cpp n_body/model/calculation/BarnesHut/BarnesHutSolver.h n_body/view/Recorder.cpp n_body/view/Recorder.h n_body/Controller.cpp n_body/Controller.h n_body/scenarios/Preset.cpp n_body/scenarios/Preset.h n_body/scenarios/ScenarioStream.cpp n_body/scenarios/ScenarioStream.h n_body/controller/Application.cpp n_body/controller/Application.h n_body/controller/Window.cpp n_body/controller/Window.h n_body/controller/pages/physics_page/PhysicsContextCreator.cpp n_body/controller/pages/physics_page/PhysicsContextCreator.h n_body/controller/pages/view_page/ViewCreator.cpp n_body/controller/pages/view_page/ViewCreator.h n_body/controller/pages/solver_page/SolverCreator.cpp n_body/controller/pages/solver_page/SolverCreator.h n_body/tools/Position.cpp n_body/tools/Position.h n_body/tools/Velocity.cpp n_body/tools/Velocity.h n_body/tools/rgbaColor.cpp n_body/tools/rgbaColor.h)

# Sets OpenMP compilation flags to allow for multithreaded performance -mavx allows avx instructions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Og ${OpenMP_CXX_FLAGS}")

# Adds libraries to compilation path
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES} raylib)

