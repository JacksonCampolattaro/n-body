
# Sets CMake standard
set(CMAKE_CXX_STANDARD 14)

# Confirms that OpenMP is present and installed properly
find_package(OpenMP REQUIRED)

# Confirms that OpenGL libraries are present and installed properly
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Confirms that controller libraries are present and installed properly
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# Adds my files to compilation path
add_executable(${CMAKE_PROJECT_NAME} n_body/main.cpp n_body/view/GLToolkit.cpp n_body/view/GLToolkit.h n_body/view/Drawable.cpp n_body/view/Drawable.h n_body/model/calculation/BarnesHut/Octant.cpp n_body/model/calculation/BarnesHut/Octant.h n_body/model/Body.cpp n_body/model/Body.h n_body/model/calculation/Solver.cpp n_body/model/calculation/Solver.h n_body/model/calculation/Naive/NaiveSolver.cpp n_body/model/calculation/Naive/NaiveSolver.h n_body/model/PhysicsContext.cpp n_body/model/PhysicsContext.h n_body/model/Model.cpp n_body/model/Model.h n_body/view/View.cpp n_body/view/View.h n_body/model/calculation/BarnesHut/BarnesHutSolver.cpp n_body/model/calculation/BarnesHut/BarnesHutSolver.h n_body/view/Recorder.cpp n_body/view/Recorder.h n_body/Controller.cpp n_body/Controller.h n_body/Scenarios/Preset.cpp n_body/Scenarios/Preset.h n_body/controller/Application.cpp n_body/controller/Application.h n_body/controller/Window.cpp n_body/controller/Window.h n_body/controller/pages/physics_page/PhysicsContextCreator.cpp n_body/controller/pages/physics_page/PhysicsContextCreator.h n_body/controller/pages/view_page/ViewCreator.cpp n_body/controller/pages/view_page/ViewCreator.h n_body/controller/pages/view_page/RecorderCreator.cpp n_body/controller/pages/view_page/RecorderCreator.h n_body/controller/pages/solver_page/SolverCreator.cpp n_body/controller/pages/solver_page/SolverCreator.h n_body/Scenarios/ScenarioStream.cpp n_body/Scenarios/ScenarioStream.h n_body/tools/Position.cpp n_body/tools/Position.h n_body/tools/Velocity.cpp n_body/tools/Velocity.h n_body/tools/Color.cpp n_body/tools/Color.h n_body/view/rendering/Renderer.cpp n_body/view/rendering/Renderer.h n_body/view/rendering/BodyRenderer.cpp n_body/view/rendering/BodyRenderer.h)

# Sets OpenMP compilation flags to allow for multithreaded performance -mavx allows avx instructions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx ${OpenMP_CXX_FLAGS}")

# Addls OpenGL and tbb libraries to compilation path
target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES} ${GTKMM_LIBRARIES} glfw)

