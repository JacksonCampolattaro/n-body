
# Sets CMake standard
set(CMAKE_CXX_STANDARD 14)

# Confirms that OpenMP is present and installed properly
find_package(OpenMP REQUIRED)

# Handling Qt5, including its own project management system
find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        3DCore
        3DExtras
        3DRender
        3DInput
        )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Adds my files to compilation path
add_executable(${CMAKE_PROJECT_NAME}
        n_body/main.cpp

        n_body/model/calculation/BarnesHut/Octant.cpp
        n_body/model/calculation/BarnesHut/Octant.h
        n_body/model/Body.cpp
        n_body/model/Body.h
        n_body/model/calculation/Solver.cpp
        n_body/model/calculation/Solver.h
        n_body/model/calculation/Naive/NaiveSolver.cpp
        n_body/model/calculation/Naive/NaiveSolver.h
        n_body/model/PhysicsContext.cpp
        n_body/model/PhysicsContext.h
        n_body/model/calculation/BarnesHut/BarnesHutSolver.cpp
        n_body/model/calculation/BarnesHut/BarnesHutSolver.h

        n_body/Controller.cpp
        n_body/Controller.h

        n_body/scenarios/Preset.cpp
        n_body/scenarios/Preset.h
        n_body/scenarios/ScenarioStream.cpp
        n_body/scenarios/ScenarioStream.h

        n_body/tools/Position.cpp
        n_body/tools/Position.h
        n_body/tools/Velocity.cpp
        n_body/tools/Velocity.h
        n_body/tools/rgbaColor.cpp
        n_body/tools/rgbaColor.h

        n_body/view/ViewportWidget.cpp
        n_body/view/ViewportWidget.h n_body/view/Launcher.cpp n_body/view/Launcher.h)

# Sets OpenMP compilation flags to allow for multithreaded performance -mavx allows avx instructions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 ${OpenMP_CXX_FLAGS}")

# Adds libraries to compilation path
target_link_libraries(${CMAKE_PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::3DCore
        Qt5::3DExtras
        Qt5::3DRender
        Qt5::3DInput
        )

