//
// Created by Jackson Campolattaro on 2/2/23.
//

#ifndef N_BODY_GRADER_H
#define N_BODY_GRADER_H

#include "NBody/Simulation/Solvers/NaiveSolver.h"

#include <utility>
#include <filesystem>

namespace NBody {

    class Grader {
    protected:

        Simulation _scenario;
        Physics::Gravity _rule{};

    public:

        explicit Grader(const std::filesystem::path &scenarioPath) {
            std::ifstream scenarioFile{scenarioPath};
            assert(scenarioFile.is_open());

            // Set _scenario and fill the reference simulation
            if (scenarioPath.extension().string() == ".json") {
                nlohmann::json jsonScenario;
                scenarioFile >> jsonScenario;
                from_json(jsonScenario, _scenario);
            } else if (scenarioPath.extension().string() == ".bin" || scenarioPath.extension().string() == ".std") {
                from_tipsy(scenarioFile, _scenario);
                spdlog::info("loaded {} particles", _scenario.particleCount());
            } else {
                spdlog::error("Unrecognized file extension");
                exit(1);
            }

        }

        explicit Grader(json jsonScenario, Physics::Gravity rule = Physics::Gravity{}) : _rule(rule) {
            from_json(jsonScenario, _scenario);
        }

        explicit Grader(Simulation scenario, Physics::Gravity rule = Physics::Gravity{}) :
                _scenario(scenario), _rule(rule) {}

        [[nodiscard]] const Simulation &scenario() const { return _scenario; }

        [[nodiscard]] const Physics::Gravity &rule() const { return _rule; }

        template<typename CandidateSolver>
        [[nodiscard]] float optimalTheta(std::pair<float, float> range = {0.1, 1.0}) const {

            // Binary search is partitioned around this value
            float middleValue = (range.first + range.second) / 2.0f;

            // After we've narrowed the range down enough, we can take the lower value
            // (it's guaranteed to be within allowable error)
            if (range.second - range.first < 0.005f)
                return range.first;

            Gravity rule = this->rule();
            Simulation candidate = _scenario;

            ReplaceRule<CandidateSolver, Gravity> candidateSolver{candidate, rule};
            spdlog::trace("Testing theta value: {}", middleValue);
            candidateSolver.descentCriterion().theta() = middleValue;
            candidateSolver.updateAccelerations();

            if (acceptable(candidate))
                return optimalTheta<CandidateSolver>({middleValue, range.second});
            else
                return optimalTheta<CandidateSolver>({range.first, middleValue});
        }

        [[nodiscard]] virtual float error(const Simulation &candidateSimulation) const = 0;

        [[nodiscard]] virtual bool acceptable(const Simulation &candidateSimulation) const = 0;

    };

    class NaiveReferenceGrader : public Grader {
    protected:

        Simulation _referenceSimulation{};

    public:

        explicit NaiveReferenceGrader(const std::filesystem::path &scenarioPath) :
                Grader(scenarioPath) {

            auto referencePath = scenarioPath;
            referencePath.replace_extension(".1.json");

            // If we've saved the reference results before, load those
            if (std::filesystem::exists(referencePath)) {
                spdlog::debug("Loading naive results from a previous run");
                json referenceScenario;
                std::ifstream referenceFile{referencePath};
                referenceFile >> referenceScenario;
                from_json(referenceScenario, _referenceSimulation);
            } else {
                // Otherwise, we need to produce the reference simulation using the naive solver
                spdlog::debug("Performing Naive step, for accuracy reference");
                _referenceSimulation = _scenario;
                NaiveSolver<Physics::Gravity> referenceSolver{_referenceSimulation, _rule};
                referenceSolver.updateAccelerations();

                // Save the reference results, so we don't need to do this again next time
                spdlog::debug("Saving naive results for future runs");
                json referenceScenario;
                to_json(referenceScenario, _referenceSimulation);
                std::ofstream referenceFile{referencePath};
                referenceFile << referenceScenario;

            }

        }

        explicit NaiveReferenceGrader(json scenario, Physics::Gravity rule = Physics::Gravity{}) :
                Grader(scenario, rule) {

            // Load the scenario
            from_json(_scenario, _referenceSimulation);

            // Run the simulation one step, to prepare the reference
            spdlog::debug("Performing Naive step, for accuracy reference");
            NaiveSolver<Physics::Gravity> referenceSolver{_referenceSimulation, _rule};
            referenceSolver.updateAccelerations();
        }

        explicit NaiveReferenceGrader(const Simulation& scenario, Physics::Gravity rule = Physics::Gravity{}) :
                Grader(scenario, rule), _referenceSimulation(scenario) {

            // Run the simulation one step, to prepare the reference
            spdlog::debug("Performing Naive step, for accuracy reference");
            NaiveSolver<Physics::Gravity> referenceSolver{_referenceSimulation, _rule};
            referenceSolver.updateAccelerations();
        }

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

    public:

        float error(const Simulation &candidateSimulation) const override = 0;

        bool acceptable(const Simulation &candidateSimulation) const override = 0;

        virtual float error(const Simulation &A, Entity a, const Simulation &B, Entity b) const = 0;

    };

}

#endif //N_BODY_GRADER_H
