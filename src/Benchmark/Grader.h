//
// Created by Jackson Campolattaro on 2/2/23.
//

#ifndef N_BODY_GRADER_H
#define N_BODY_GRADER_H

#include "Comparison.h"

#include <NBody/Simulation/Solvers/NaiveSolver.h>

#include <utility>

namespace NBody {

    class Grader {
    protected:

        json _scenario;
        Physics::Rule _rule{};
        Simulation _referenceSimulation{};

    public:

        explicit Grader(json scenario, Physics::Rule rule = Physics::Rule{}) :
                _scenario(scenario), _rule(rule) {

            // Load the scenario
            from_json(_scenario, _referenceSimulation);

            // Run the simulation one step, to prepare the reference
            spdlog::debug("Performing Naive step, for accuracy reference");
            NaiveSolver referenceSolver{_referenceSimulation, _rule};
            referenceSolver.step();
        }

        const json &scenario() const { return _scenario; }

        Physics::Rule &rule() { return _rule; }

        const Physics::Rule &rule() const { return _rule; }

        virtual float error(const Simulation &candidateSimulation) const = 0;

        float averageError(const Simulation &candidateSimulation) const {
            auto referenceValues = _referenceSimulation.view<const Physics::Acceleration>();
            auto candidateValues = candidateSimulation.view<const Physics::Acceleration>();
            return std::transform_reduce(referenceValues.begin(), referenceValues.end(), candidateValues.begin(), 0.0f,
                                         std::plus<>(),
                                         [&](const auto &r, const auto &c) {
                                             return error(_referenceSimulation, r, candidateSimulation, c);
                                         }
            ) / (float) referenceValues.size();
        }

        float maximumError(const Simulation &candidateSimulation) const {
            auto referenceValues = _referenceSimulation.view<const Physics::Acceleration>();
            auto candidateValues = candidateSimulation.view<const Physics::Acceleration>();
            return std::transform_reduce(referenceValues.begin(), referenceValues.end(), candidateValues.begin(), 0.0f,
                                         [&](float e1, float e2) {
                                             return std::max(e1, e2);
                                         },
                                         [&](const auto &r, const auto &c) {
                                             return error(_referenceSimulation, r, candidateSimulation, c);
                                         }
            );
        }

        virtual float error(const Simulation &A, Entity a, const Simulation &B, Entity b) const = 0;

    };

}

#endif //N_BODY_GRADER_H
