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

void sampleField(const std::string &name, const std::function<Acceleration(Position)> &field,
                 const BoundingBox &sampleRegion, float sampleResolution) {

    std::ofstream out{fmt::format(
            "benchmarks/sample-{}.csv", name
    )};
    out << "x,y,z,fx,fy,fz\n";

    for (auto position: range3D(sampleRegion, sampleResolution)) {
        auto f = field(position);
        out << fmt::format(
                "{},{},{},{},{},{}\n",
                position.x, position.y, position.z,
                f.x, f.y, f.z
        );
    }
}

void sampleError(const std::string &name,
                 const std::function<Acceleration(Position)> &field,
                 const std::function<Acceleration(Position)> &exactField,
                 const BoundingBox &sampleRegion, float sampleResolution) {

    std::ofstream out{fmt::format(
            "benchmarks/sample-{}-error.csv", name
    )};
    out << "x,y,z,ex,ey,ez,e\n";

    for (auto position: range3D(sampleRegion, sampleResolution)) {
        auto e = exactField(position) - field(position);
        out << fmt::format(
                "{},{},{},{},{},{},{}\n",
                position.x, position.y, position.z,
                e.x, e.y, e.z,
                glm::length(e) / glm::length((glm::vec3) exactField(position))
        );
    }
}

void sampleDifference(const std::string &name,
                      const std::function<Acceleration(Position)> &modulationField,
                      const std::function<Acceleration(Position)> &baseField,
                      const BoundingBox &sampleRegion, float sampleResolution) {

    std::ofstream out{fmt::format(
            "benchmarks/sample-{}.csv", name
    )};
    out << "x,y,z,fx,fy,fz\n";

    for (auto position: range3D(sampleRegion, sampleResolution)) {
        auto f = modulationField(position) - baseField(position);
        out << fmt::format(
                "{},{},{},{},{},{}\n",
                position.x, position.y, position.z,
                f.x, f.y, f.z
        );
    }
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    Gravity rule{};
    Simulation trio;
    trio.newParticle().setPosition({0.0f, -2.0f, 0.0f}).setMass(2.0f);
    trio.newParticle().setPosition({-1.0f, 2.0f, 0.0f}).setMass(1.0f);
    trio.newParticle().setPosition({1.0f, 2.0f, 0.0f}).setMass(1.0f);
    auto activeView = trio.view<const Position, const Mass>();
    auto activeList = std::vector<Entity>{activeView.begin(), activeView.end()};


    BoundingBox fullRange{{-10, -10, 0},
                          {10,  10,  0}};
    float fullRangeResolution = 0.05;

    BoundingBox zoomRange{{4, -1, 0},
                          {6, 1,  0}};
    float zoomRangeResolution = 0.01;

    BoundingBox extremeZoomRange{{4.9, -0.1, 0},
                          {5.1, 0.1,  0}};
    float extremeZoomRangeResolution = 0.001;

    BoundingBox extendedRange{{-100, -100, 0},
                              {100,  100,  0}};
    float extendedRangeResolution = 0.5;


    auto exactField = [&](Position p) -> Acceleration {
        return std::transform_reduce(
                activeView.begin(), activeView.end(), Acceleration{}, std::plus<>{},
                [&](auto e) {
                    return rule(activeView.get<const Position>(e), activeView.get<const Mass>(e), p);
                }
        );
    };
    sampleField("exact", exactField, fullRange, fullRangeResolution);
    sampleField("exact-zoom", exactField, zoomRange, zoomRangeResolution);

    // Moment approximations
    {

        CenterOfMassSummary monopoleMassSummary;
        monopoleMassSummary.summarize(activeList, activeView);
        auto monopoleMomentField = [&](Position p) -> Acceleration {
            return rule(monopoleMassSummary.centerOfMass(), monopoleMassSummary, p);
        };
        sampleField("monopole-moment", monopoleMomentField, fullRange, fullRangeResolution);
        sampleError("monopole-moment", monopoleMomentField, exactField, fullRange, fullRangeResolution);
        sampleError("monopole-moment-extended", monopoleMomentField, exactField, extendedRange,
                    extendedRangeResolution);

        QuadrupoleMassSummary quadrupoleMassSummary;
        quadrupoleMassSummary.summarize(activeList, activeView);
        auto quadrupoleMomentField = [&](Position p) -> Acceleration {
            return rule(quadrupoleMassSummary.centerOfMass(), quadrupoleMassSummary, p);
        };
        sampleField("net-quadrupole-moment", quadrupoleMomentField, fullRange, fullRangeResolution);
        sampleDifference("quadrupole-moment", quadrupoleMomentField, monopoleMomentField, fullRange,
                         fullRangeResolution);
        sampleError("quadrupole-moment", quadrupoleMomentField, exactField, fullRange, fullRangeResolution);

        OctupoleMassSummary octupoleMassSummary;
        octupoleMassSummary.summarize(activeList, activeView);
        auto octupoleMomentField = [&](Position p) -> Acceleration {
            return rule(octupoleMassSummary.centerOfMass(), octupoleMassSummary, p);
        };
        sampleDifference("octupole-moment", octupoleMomentField, quadrupoleMomentField, fullRange, fullRangeResolution);
        sampleError("octupole-moment", octupoleMomentField, exactField, fullRange, fullRangeResolution);


        HexadecupoleMassSummary hexadecupoleMassSummary;
        hexadecupoleMassSummary.summarize(activeList, activeView);
        auto hexadecupoleMomentField = [&](Position p) -> Acceleration {
            return rule(hexadecupoleMassSummary.centerOfMass(), hexadecupoleMassSummary, p);
        };
        sampleDifference("hexadecupole-moment", hexadecupoleMomentField, octupoleMomentField, fullRange,
                         fullRangeResolution);
        sampleError("hexadecupole-moment", hexadecupoleMomentField, exactField, fullRange, fullRangeResolution);
    }

    // Field approximations
    {
        auto samplePosition = zoomRange.center();

        glm::vec3 netApproximateAcceleration{};
        QuadrupoleAccelerationSummary netQuadrupoleAcceleration{};
        OctupoleAccelerationSummary netOctupoleAcceleration{};
        HexadecupoleAccelerationSummary netHexadecupoleAcceleration{};
        TriacontadyupoleAccelerationSummary netTriacontadyupoleAcceleration{};
        activeView.each([&](const Position &activePosition, const Mass &activeMass) {
            netApproximateAcceleration += (glm::vec3) rule(activePosition, activeMass, samplePosition);
            rule(activePosition, activeMass, samplePosition, netQuadrupoleAcceleration);
            rule(activePosition, activeMass, samplePosition, netOctupoleAcceleration);
            rule(activePosition, activeMass, samplePosition, netHexadecupoleAcceleration);
            rule(activePosition, activeMass, samplePosition, netTriacontadyupoleAcceleration);
        });

        auto approximateFunction = [&](Position p) -> Acceleration {
            return netApproximateAcceleration;
        };
        sampleField("vector-field", approximateFunction, zoomRange, zoomRangeResolution);
        sampleError("vector-field", approximateFunction, exactField, zoomRange, zoomRangeResolution);

        auto quadrupoleApproximateFunction = [&](Position p) -> Acceleration {
            return netQuadrupoleAcceleration.acceleration().at(p - samplePosition);
        };
        sampleField("net-quadrupole-field", quadrupoleApproximateFunction,
                    zoomRange, zoomRangeResolution);
        sampleDifference("quadrupole-field", quadrupoleApproximateFunction, approximateFunction,
                         zoomRange, zoomRangeResolution);
        sampleError("quadrupole-field", quadrupoleApproximateFunction, approximateFunction,
                    zoomRange, zoomRangeResolution);
        sampleError("quadrupole-field-zoom", quadrupoleApproximateFunction, approximateFunction,
                    extremeZoomRange, extremeZoomRangeResolution);

        auto octupoleApproximateFunction = [&](Position p) -> Acceleration {
            return netOctupoleAcceleration.acceleration().at(p - samplePosition);
        };
        sampleDifference("octupole-field", octupoleApproximateFunction, quadrupoleApproximateFunction,
                         zoomRange, zoomRangeResolution);
        sampleError("octupole-field", octupoleApproximateFunction, quadrupoleApproximateFunction,
                    zoomRange, zoomRangeResolution);

        auto hexadecupoleApproximateFunction = [&](Position p) -> Acceleration {
            return netHexadecupoleAcceleration.acceleration().at(p - samplePosition);
        };
        sampleDifference("hexadecupole-field", hexadecupoleApproximateFunction, octupoleApproximateFunction,
                         zoomRange, zoomRangeResolution);
        sampleError("hexadecupole-field", hexadecupoleApproximateFunction, octupoleApproximateFunction,
                    zoomRange, zoomRangeResolution);

        auto triacontadyupoleApproximateFunction = [&](Position p) -> Acceleration {
            return netTriacontadyupoleAcceleration.acceleration().at(p - samplePosition);
        };
        sampleDifference("triacontadyupole-field", triacontadyupoleApproximateFunction, hexadecupoleApproximateFunction,
                         zoomRange, zoomRangeResolution);
        sampleError("triacontadyupole-field", triacontadyupoleApproximateFunction, hexadecupoleApproximateFunction,
                    zoomRange, zoomRangeResolution);
    }

}
