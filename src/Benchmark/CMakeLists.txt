
# Add necessary libraries
find_package(spdlog REQUIRED)
find_package(Matplot++ REQUIRED)

add_executable(Benchmark
        main.cpp Comparison.h Generator.h ConstitutionalGrader.h Grader.h bestTheta.h RMSGrader.h MeanGrader.h)

target_link_libraries(Benchmark PUBLIC

        NBody::Simulation
        NBody::View

        spdlog::spdlog
        Matplot++::matplot
        )
