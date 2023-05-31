//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_BESTTHETA_H
#define N_BODY_BESTTHETA_H

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "ConstitutionalGrader.h"
#include "RMSGrader.h"

using namespace NBody;

template<typename CandidateSolver>
inline float searchTheta(json scenario, const Grader &grader,
                         std::pair<float, float> range = {0.1, 1.0}) {

    // Binary search is partitioned around this value
    float middleValue = (range.first + range.second) / 2.0f;

    // After we've narrowed the range down enough, we can take the lower value
    // (it's guaranteed to be within allowable error)
    if (range.second - range.first < 0.005f)
        return range.first;

    typename CandidateSolver::Rule rule{grader.rule()};
    Simulation candidate;
    from_json(scenario, candidate);

    CandidateSolver candidateSolver{candidate, rule};
    candidateSolver.descentCriterion().theta() = middleValue;
    candidateSolver.step();

    float error = grader.error(candidate);

    spdlog::debug("theta = {} --> error = {}", middleValue, error);

    if (error <= 0.5)
        return searchTheta<CandidateSolver>(scenario, grader, {middleValue, range.second});
    else
        return searchTheta<CandidateSolver>(scenario, grader, {range.first, middleValue});
}

template<typename CandidateSolver>
inline float searchTheta(const Grader &grader,
                         std::pair<float, float> range = {0.1, 1.0}) {
    return searchTheta<CandidateSolver>(grader.scenario(), grader, range);
}

template<typename CandidateSolver>
inline float bestTheta() {

    // Load the test scenario
    Simulation s;
    std::ifstream file{"LOW.bin"};
    from_tipsy(file, s);
    json scenario;
    to_json(scenario, s);

    // Produce a grader based on the scenario
    RMSGrader grader{scenario};

    // Perform a binary search of possible theta values
    return searchTheta<CandidateSolver>(scenario, grader);
}


#endif //N_BODY_BESTTHETA_H
