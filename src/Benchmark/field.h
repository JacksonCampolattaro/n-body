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
            "benchmarks/field/sample-{}.csv", name
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

#endif //N_BODY_FIELD_H
