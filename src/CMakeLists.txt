
# Sets CMake standard
set(CMAKE_CXX_STANDARD 14)

# Confirms that OpenMP is present and installed properly
find_package(OpenMP REQUIRED)

# Confirms that OpenGL libraries are present and installed properly
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Confirms that Gtkmm libraries are present and installed properly
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# Adds my files to compilation path
add_executable(${CMAKE_PROJECT_NAME} n_body/main.cpp n_body/view/GLToolkit.cpp n_body/view/GLToolkit.h n_body/view/Drawable.cpp n_body/view/Drawable.h n_body/model/calculation/BarnesHut/Octant.cpp n_body/model/calculation/BarnesHut/Octant.h n_body/model/Body.cpp n_body/model/Body.h n_body/model/calculation/Solver.cpp n_body/model/calculation/Solver.h n_body/model/calculation/Naive/NaiveSolver.cpp n_body/model/calculation/Naive/NaiveSolver.h n_body/model/PhysicsContext.cpp n_body/model/PhysicsContext.h n_body/model/Model.cpp n_body/model/Model.h n_body/view/View.cpp n_body/view/View.h n_body/model/calculation/BarnesHut/BarnesHutSolver.cpp n_body/model/calculation/BarnesHut/BarnesHutSolver.h n_body/view/Recorder.cpp n_body/view/Recorder.h n_body/Controller.cpp n_body/Controller.h n_body/Launcher/Launcher.cpp n_body/Launcher/Launcher.h n_body/Launcher/Preset.cpp n_body/Launcher/Preset.h n_body/Launcher/ViewSettingsMenu.cpp n_body/Launcher/ViewSettingsMenu.h n_body/Launcher/RecorderSettingsMenu.cpp n_body/Launcher/RecorderSettingsMenu.h n_body/Launcher/PhysicsSettingsMenu.cpp n_body/Launcher/PhysicsSettingsMenu.h n_body/Gtkmm/nbody_application.cpp n_body/Gtkmm/nbody_application.h n_body/Gtkmm/nbody_window.cpp n_body/Gtkmm/nbody_window.h n_body/Gtkmm/pages/physics_page/physicsContext_creator.cpp n_body/Gtkmm/pages/physics_page/physicsContext_creator.h n_body/Gtkmm/pages/view_page/view_creator.cpp n_body/Gtkmm/pages/view_page/view_creator.h n_body/Gtkmm/pages/view_page/recorder_creator.cpp n_body/Gtkmm/pages/view_page/recorder_creator.h)

# Sets OpenMP compilation flags to allow for multithreaded performance -mavx allows avx instructions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx ${OpenMP_CXX_FLAGS}")

# Addls OpenGL and tbb libraries to compilation path
target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES} ${GTKMM_LIBRARIES} glfw tbb)

