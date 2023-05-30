
# Add necessary libraries
find_package(spdlog REQUIRED)
find_package(Matplot++ REQUIRED)

add_executable(Benchmark
        main.cpp Comparison.h Generator.h ConstitutionalGrader.h Grader.h bestTheta.h RMSGrader.h MeanGrader.h fieldPlot.h field.h)

add_executable(Profile
        profiling/main.cpp Generator.h)

add_executable(CountInteractions interaction-counts/main.cpp Generator.h)

target_link_libraries(Benchmark PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        Matplot++::matplot
        )

target_link_libraries(Profile PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        )

target_link_libraries(CountInteractions PUBLIC
        NBody::Simulation
        NBody::View
        spdlog::spdlog
        )
