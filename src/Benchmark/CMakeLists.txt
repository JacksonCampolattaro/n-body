
# Add necessary libraries
find_package(spdlog REQUIRED)

add_executable(Profile
        profile.cpp Generator.h)
target_link_libraries(Profile PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(Test
        test.cpp Generator.h)
target_link_libraries(Test PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(CountInteractions countInteractions.cpp Generator.h)
target_link_libraries(CountInteractions PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )


### Experiments

add_executable(experiment_CompareLinearBVHDescentCriterion
        Experiments/compareLinearBVHDescentCriterion.cpp)
target_link_libraries(experiment_CompareLinearBVHDescentCriterion PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(experiment_CompareSolversOnRandomData
        Experiments/compareSolversOnRandomData.cpp)
target_link_libraries(experiment_CompareSolversOnRandomData PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(experiment_CompareSolversOnAGORAData
        Experiments/compareSolversOnAGORAData.cpp)
target_link_libraries(experiment_CompareSolversOnAGORAData PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(experiment_CompareTreeConstructionOnAGORAData
        Experiments/compareTreeConstructionOnAGORAData.cpp)
target_link_libraries(experiment_CompareTreeConstructionOnAGORAData PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(experiment_CompareTreeConstructionOnRandomData
        Experiments/compareTreeConstructionOnRandomData.cpp)
target_link_libraries(experiment_CompareTreeConstructionOnRandomData PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(experiment_recordInteractions
        Experiments/recordInteractions.cpp)
target_link_libraries(experiment_recordInteractions PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(experiment_countInteractions
        Experiments/countInteractions.cpp)
target_link_libraries(experiment_countInteractions PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(experiment_sampleMultipoles
        Experiments/sampleMultipoles.cpp)
target_link_libraries(experiment_sampleMultipoles PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(experiment_sampleFieldTrees
        Experiments/sampleFieldTrees.cpp)
target_link_libraries(experiment_sampleFieldTrees PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )

add_executable(experiment_CompareThetaValues
        Experiments/compareThetaValues.cpp)
target_link_libraries(experiment_CompareThetaValues PUBLIC
        NBody::Simulation
        spdlog::spdlog
        )
