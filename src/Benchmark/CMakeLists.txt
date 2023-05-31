
# Add necessary libraries
find_package(spdlog REQUIRED)
find_package(Matplot++ REQUIRED)

add_executable(Benchmark
        main.cpp Comparison.h Generator.h ConstitutionalGrader.h Grader.h bestTheta.h RMSGrader.h MeanGrader.h fieldPlot.h field.h)
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

add_executable(experiment_CompareLinearBVHDescentCriterion
        Experiments/compareLinearBVHDescentCriterion.cpp)
target_link_libraries(experiment_CompareLinearBVHDescentCriterion PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        )

add_executable(experiment_CompareAllSolversOnRandomData
        Experiments/compareSolversOnRandomData.cpp)
target_link_libraries(experiment_CompareAllSolversOnRandomData PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        )
