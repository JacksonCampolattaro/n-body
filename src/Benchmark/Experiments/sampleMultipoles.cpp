//
// Created by Jackson Campolattaro on 6/15/23.
//
#include <gtkmm.h>
#include <spdlog/spdlog.h>

#include <NBody/Physics/Rules/Gravity.h>
#include <NBody/Physics/Rules/SimpleTrackingRule.h>

#include <NBody/Simulation/Simulation.h>

#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/ImplicitReverseBarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/ImplicitFMMSolver.h>
#include <NBody/Simulation/Solvers/ImplicitMVDRSolver.h>

#include "../Generator.h"
#include "../bestTheta.h"
#include "../benchmark.h"

std::vector<glm::vec3> range3D(const BoundingBox &sampleRegion, float sampleResolution) {
    std::vector<glm::vec3> samples;
    for (float x = sampleRegion.min().x; x <= sampleRegion.max().x + sampleResolution; x += sampleResolution)
        for (float y = sampleRegion.min().y; y <= sampleRegion.max().y + sampleResolution; y += sampleResolution)
            for (float z = sampleRegion.min().z; z <= sampleRegion.max().z; z += sampleResolution)
                samples.emplace_back(x, y, z);

    return samples;
}

Acceleration exactField(const Simulation &simulation, const glm::vec3 &samplePosition) {
    Gravity rule{};
    auto activeView = simulation.view<const Position, const Mass>();
    return std::transform_reduce(
            activeView.begin(), activeView.end(), Acceleration{}, std::plus<>{},
            [&](auto e) {
                return rule(activeView.get<const Position>(e), activeView.get<const Mass>(e), samplePosition);
            }
    );
}

void sampleExactField(const Simulation &simulation, const BoundingBox &sampleRegion, float sampleResolution) {

    std::ofstream out{fmt::format(
            "benchmarks/sample-exact-{}x{}.csv",
            sampleRegion.dimensions().x, sampleRegion.dimensions().y
    )};
    out << "x,y,z,fx,fy,fz\n";

    for (auto position: range3D(sampleRegion, sampleResolution)) {
        auto field = exactField(simulation, position);
        out << fmt::format(
                "{},{},{},{},{},{}\n",
                position.x, position.y, position.z,
                field.x, field.y, field.z
        );
    }
}

template<std::size_t Order>
void sampleMultipoleMoment() {

    std::ofstream out{fmt::format("benchmarks/sample-{}-moment.csv", Multipole<Order>::name())};

    MultipoleMoment<Order> moment;
}

template<std::size_t Order>
void sampleMultipoleField() {

}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    Simulation trio;
    trio.newParticle().setPosition({0.0f, -2.0f, 0.0f}).setMass(2.0f);
    trio.newParticle().setPosition({-1.0f, 2.0f, 0.0f}).setMass(1.0f);
    trio.newParticle().setPosition({1.0f, 2.0f, 0.0f}).setMass(1.0f);

    BoundingBox fullRange{{-4, -4, 0},
                          {4,  4,  0}};
    float fullRangeResolution = 0.04;
    BoundingBox zoomRange{{1.5f, -0.5f, 0},
                          {2.5f, 0.5f, 0}};
    float zoomRangeResolution = 0.01;

    sampleExactField(trio, fullRange, fullRangeResolution);
    sampleExactField(trio, zoomRange, zoomRangeResolution);
}
