
# Sets CMake standard
set(CMAKE_CXX_STANDARD 14)

# Confirms that OpenMP is present and installed properly
find_package(OpenMP REQUIRED)
#find_package(raylib 2.0 QUIET)

# Confirms that controller libraries are present and installed properly
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# Adds my files to compilation path
add_executable(${CMAKE_PROJECT_NAME}
        main.cpp

        model/calculation/BarnesHut/Octant.cpp
        model/calculation/BarnesHut/Octant.h
        model/Body.cpp
        model/Body.h
        model/calculation/Solver.cpp
        model/calculation/Solver.h
        model/calculation/Naive/NaiveSolver.cpp
        model/calculation/Naive/NaiveSolver.h
        model/PhysicsContext.cpp
        model/PhysicsContext.h
        model/calculation/BarnesHut/BarnesHutSolver.cpp
        model/calculation/BarnesHut/BarnesHutSolver.h

        view/Drawable.cpp
        view/Drawable.h
        view/View.cpp
        view/View.h
        view/Recorder.cpp
        view/Recorder.h

        Controller.cpp
        Controller.h
        controller/Application.cpp
        controller/Application.h
        controller/Window.cpp
        controller/Window.h
        controller/pages/physics_page/PhysicsContextCreator.cpp
        controller/pages/physics_page/PhysicsContextCreator.h
        controller/pages/view_page/ViewCreator.cpp
        controller/pages/view_page/ViewCreator.h
        controller/pages/solver_page/SolverCreator.cpp
        controller/pages/solver_page/SolverCreator.h

        model/tools/Position.cpp
        model/tools/Position.h
        model/tools/Velocity.cpp
        model/tools/Velocity.h
        model/tools/rgbaColor.cpp
        model/tools/rgbaColor.h
        model/tools/BodyList.cpp
        model/tools/BodyList.h

        ../scenarios/Preset.cpp
        ../scenarios/Preset.h )

# Sets OpenMP compilation flags to allow for multithreaded performance -mavx allows avx instructions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Og ${OpenMP_CXX_FLAGS}")

# Adds libraries to compilation path
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES} raylib)

