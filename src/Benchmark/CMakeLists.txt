
# Add necessary libraries
find_package(spdlog REQUIRED)
find_package(Matplot++ REQUIRED)

add_executable(Benchmark
        main.cpp Generator.h Graders/ConstitutionalGrader.h Graders/Grader.h bestTheta.h Graders/RMSGrader.h Graders/MeanGrader.h fieldPlot.h field.h)
target_link_libraries(Benchmark PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        Matplot++::matplot
        )

add_executable(Profile
        profile.cpp Generator.h)
target_link_libraries(Profile PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        )

add_executable(CountInteractions countInteractions.cpp Generator.h)
target_link_libraries(CountInteractions PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        )


### Experiments

add_executable(experiment_CompareLinearBVHDescentCriterion
        Experiments/compareLinearBVHDescentCriterion.cpp)
target_link_libraries(experiment_CompareLinearBVHDescentCriterion PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        )

add_executable(experiment_CompareSolversOnRandomData
        Experiments/compareSolversOnRandomData.cpp)
target_link_libraries(experiment_CompareSolversOnRandomData PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        )

add_executable(experiment_CompareSolversOnAGORAData
        Experiments/compareSolversOnAGORAData.cpp)
target_link_libraries(experiment_CompareSolversOnAGORAData PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        )

add_executable(experiment_CompareTreeConstruction
        Experiments/compareTreeConstruction.cpp)
target_link_libraries(experiment_CompareTreeConstruction PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        )
