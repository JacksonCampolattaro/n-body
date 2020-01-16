

# Sets CMake standard
set(CMAKE_CXX_STANDARD 17)

# Confirms that Magnum is present and installed properly
find_package(Magnum REQUIRED
        GL
        SceneGraph
        Sdl2Application
        MeshTools
        Primitives
        Shaders
        )
#set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Confirms that magnum-integration is present and installed properly
find_package(MagnumIntegration REQUIRED Glm)

# Confirms that OpenMP is present and installed properly
find_package(OpenMP REQUIRED)

# Confirms that controller libraries are present and installed properly
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

        model/tools/Position.cpp
        model/tools/Position.h
        model/tools/Velocity.cpp
        model/tools/Velocity.h
        model/tools/rgbaColor.cpp
        model/tools/rgbaColor.h
        model/tools/BodyList.cpp
        model/tools/BodyList.h

        ../scenarios/Preset.cpp
        ../scenarios/Preset.h

        runner.cpp
        runner.h

        view/Viewport.cpp
        view/Viewport.h
        view/Viewport.cpp view/Viewport.h Config.cpp Config.h view/ColoredDrawable.cpp view/ColoredDrawable.h view/BodyObject.cpp view/BodyObject.h)

# Adds libraries to compilation path
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE

        Magnum::Application
        Magnum::GL
        Magnum::Magnum
        Magnum::SceneGraph
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::Shaders

        MagnumIntegration::Glm

        ${GTKMM_LIBRARIES}
        )

# Sets OpenMP compilation flags to allow for multithreaded performance -mavx allows avx instructions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Og ${OpenMP_CXX_FLAGS}")
