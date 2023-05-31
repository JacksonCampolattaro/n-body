//
// Created by Jackson Campolattaro on 5/7/23.
//

#ifndef N_BODY_FIELD_H
#define N_BODY_FIELD_H

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <glm/gtx/color_space.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "NBody/Physics/Rules/Gravity.h"

void sampleExactField(
        const json &scenario,
        const std::vector<float> &xValues,
        const std::vector<float> &yValues,
        const std::vector<float> &zValues = {0.0f}
) {

    std::ofstream file{"benchmarks/exact-field.csv"};
    file << "x,y,z,fx,fy,fz" << std::endl;

    Gravity rule{};
    Simulation simulation;
    from_json(scenario, simulation);

    for (auto x: xValues) {
        for (auto y: yValues) {
            for (auto z: zValues) {

                auto actorsView = simulation.view<const Position, const Mass>();
                glm::vec3 f = {};
                actorsView.each([&](const Position &activePosition, const Mass &activeMass) {
                    f += (glm::vec3) rule(activePosition, activeMass, Position{x, y, z});
                });


                file << fmt::format(
                        "{},{},{},{},{},{}\n",
                        x, y, z, f.x, f.y, f.z
                );

            }
        }
    }

}

void sampleExactField(
        const json &scenario,
        const std::pair<float, float> &xRange = {-10.0f, 10.0f},
        const std::pair<float, float> &yRange = {-10.0f, 10.0f},
        const std::pair<float, float> &zRange = {0.0f, 0.0f},
        float epsilon = 0.1f
) {

    std::vector<float> xValues{xRange.first};
    while (xValues.back() < xRange.second)
        xValues.emplace_back(xValues.back() + epsilon);

    std::vector<float> yValues{yRange.first};
    while (yValues.back() < yRange.second)
        yValues.emplace_back(yValues.back() + epsilon);

    std::vector<float> zValues{zRange.first};
    while (zValues.back() < zRange.second)
        zValues.emplace_back(zValues.back() + epsilon);

    sampleExactField(scenario, xValues, yValues, zValues);
}

#endif //N_BODY_FIELD_H
