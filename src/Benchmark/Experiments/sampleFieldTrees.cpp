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

class NonContaining {
public:
    template<NodeType TreeNode>
    inline bool operator()(const Position &point, const TreeNode &passiveNode) const {
        return !Descent::exclusionRegion(passiveNode).contains(point);
    }
};

template<SummaryType S>
void sampleTreeField(const std::string &label, Simulation simulation, const BoundingBox &range) {

    Gravity rule{};
    PassiveTreeSolver<Octree<S>, NonContaining, Gravity> solver{simulation, rule};
    //solver.tree().maxLeafSize() = 2;
    solver.updateAccelerations();
    auto field = [&](Position p) -> Acceleration {
        return NBody::Descent::accelerationAt(solver.tree().root(), p);
    };

    float resolution = range.maxSideLength() / 512;
    sampleField(label, field, range, resolution);
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    Gravity rule{};
    Simulation simulation = Generator::createScenario(Generator::perlinNoiseRandomVolume, 100'000);
    BoundingBox fullRange = simulation.boundingBox();
    fullRange.max().z = fullRange.min().z = 50.0f;
    BoundingBox zoomRange{fullRange.min() / 10.0f, fullRange.max() / 10.0f};

    sampleTreeField<AccelerationSummary>("vector-tree", simulation, fullRange);
    sampleTreeField<QuadrupoleAccelerationSummary>("quadrupole-tree", simulation, fullRange);
    sampleTreeField<OctupoleAccelerationSummary>("octupole-tree", simulation, fullRange);
    sampleTreeField<HexadecupoleAccelerationSummary>("hexadecupole-tree", simulation, fullRange);
    sampleTreeField<TriacontadyupoleAccelerationSummary>("triacontadyupole-tree", simulation, fullRange);
}
