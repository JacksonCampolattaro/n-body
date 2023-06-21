//
// Created by Jackson Campolattaro on 6/21/23.
//
#include <gtkmm.h>
#include <spdlog/spdlog.h>

#include <NBody/Physics/Rules/Gravity.h>
#include <NBody/Physics/Rules/SimpleTrackingRule.h>

#include <NBody/Simulation/Simulation.h>

#include <NBody/Simulation/Solvers/ReverseBarnesHutSolver.h>

#include "../Generator.h"
#include "../field.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    Gravity rule{};
    Simulation simulation = Generator::createScenario(Generator::perlinNoiseRandomVolume, 100'000);
    BoundingBox fullRange = simulation.boundingBox();
    fullRange.max().z = fullRange.min().z = 0.0f;
    float fullRangeResolution = fullRange.maxSideLength() / 400;

    {
        auto r = rule;
        auto s = simulation;
        ReverseBarnesHutSolver<Gravity> solver{s, r};
        solver.descentCriterion().theta() = std::numeric_limits<float>::infinity();
        solver.tree().maxLeafSize() = 1;
        solver.updateAccelerations();
        auto field = [&](Position p) -> Acceleration {
            return NBody::Descent::accelerationAt(solver.tree().root(), p);
        };
        sampleField("vector-tree", field, fullRange, fullRangeResolution);
    }

    {
        auto r = rule;
        auto s = simulation;
        QuadrupoleReverseBarnesHutSolver<Gravity> solver{s, r};
        solver.descentCriterion().theta() = std::numeric_limits<float>::infinity();
        solver.tree().maxLeafSize() = 1;
        solver.updateAccelerations();
        auto field = [&](Position p) -> Acceleration {
            return NBody::Descent::accelerationAt(solver.tree().root(), p);
        };
        sampleField("quadrupole-tree", field, fullRange, fullRangeResolution);
    }

    {
        auto r = rule;
        auto s = simulation;
        OctupoleReverseBarnesHutSolver<Gravity> solver{s, r};
        solver.descentCriterion().theta() = std::numeric_limits<float>::infinity();
        solver.tree().maxLeafSize() = 1;
        solver.updateAccelerations();
        auto field = [&](Position p) -> Acceleration {
            return NBody::Descent::accelerationAt(solver.tree().root(), p);
        };
        sampleField("octupole-tree", field, fullRange, fullRangeResolution);
    }

    {
        auto r = rule;
        auto s = simulation;
        HexadecupoleReverseBarnesHutSolver<Gravity> solver{s, r};
        solver.descentCriterion().theta() = std::numeric_limits<float>::infinity();
        solver.tree().maxLeafSize() = 1;
        solver.updateAccelerations();
        auto field = [&](Position p) -> Acceleration {
            return NBody::Descent::accelerationAt(solver.tree().root(), p);
        };
        sampleField("hexadecupole-tree", field, fullRange, fullRangeResolution);
    }

    // todo
}
