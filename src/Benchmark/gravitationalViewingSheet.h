//
// Created by Jackson Campolattaro on 4/8/23.
//

#ifndef N_BODY_GRAVITATIONALVIEWINGSHEET_H
#define N_BODY_GRAVITATIONALVIEWINGSHEET_H

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <NBody/Physics/Rule.h>
#include <matplot/matplot.h>

using namespace NBody;
using nlohmann::json;

void plotFunctionInRange(
        matplot::axes_handle &ax,
        const std::function<float(double, double)> &function,
        const std::pair<float, float> &xRange,
        const std::pair<float, float> &yRange,
        double resolution = 0.05f
) {

    ax->xlabel("X");
    ax->ylabel("Y");

    auto [X, Y] = matplot::meshgrid(matplot::iota(xRange.first, resolution, xRange.second),
                                    matplot::iota(yRange.first, resolution, yRange.second));
    auto F = matplot::transform(X, Y, function);
    matplot::imagesc(ax, xRange.first, xRange.second, yRange.first, yRange.second, F);
}

void plotFieldLogMagnitudeInRange(
        matplot::axes_handle &ax,
        const std::function<glm::vec3(double, double)> &function,
        const std::pair<float, float> &xRange,
        const std::pair<float, float> &yRange,
        double resolution = 0.05f
) {
    auto logMagnitudeFunction = [&](double x, double y) {
        return std::log(glm::length(function(x, y)));
    };
    plotFunctionInRange(ax, logMagnitudeFunction, xRange, yRange, resolution);
}

void plotFieldErrorInRange(
        matplot::axes_handle &ax,
        const std::function<glm::vec3(double, double)> &approximateField,
        const Simulation &simulation,
        const Rule &rule,
        const std::pair<float, float> &xRange,
        const std::pair<float, float> &yRange,
        double resolution = 0.05f
) {

    auto errorFunction = [&](double x, double y) {
        auto actorsView = simulation.view<const Position, const Mass>();
        glm::vec3 netAcceleration = {};
        actorsView.each([&](const Position &activePosition, const Mass &activeMass) {
            netAcceleration += (glm::vec3) rule(activePosition, activeMass, Position{x, y, 0.0f});
        });
        return glm::length(netAcceleration - approximateField(x, y));
    };

    matplot::colormap(ax, matplot::palette::hot());
    plotFunctionInRange(ax, errorFunction, xRange, yRange, resolution);
    matplot::colorbar(ax);
}

void plotFunctionWithZoom(
        const std::function<float(double, double)> &function,
        const std::pair<float, float> &xRange,
        const std::pair<float, float> &yRange,
        const std::pair<float, float> &xZoomRange,
        const std::pair<float, float> &yZoomRange,
        double resolution = 0.5f
) {

    float zoomFactor = (xRange.second - xRange.first) / (xZoomRange.second - xZoomRange.first);

    auto f = matplot::figure();
    f->size(1000, 450);
    matplot::tiledlayout(1, 2);

    auto ax1 = matplot::nexttile();
    ax1->title("Gravitational Field Magnitude (log-scale)");
    ax1->axes_aspect_ratio_auto(false);
    plotFunctionInRange(ax1, function, xRange, yRange, resolution);
    matplot::rectangle(ax1,
                       xZoomRange.first, yZoomRange.first,
                       xZoomRange.second - xZoomRange.first, yZoomRange.second - yZoomRange.first);

    auto ax2 = matplot::nexttile();
    ax2->title("Local Field Magnitude Across Selected Region (log-scale)");
    ax2->axes_aspect_ratio_auto(false);
    plotFunctionInRange(ax2, function, xZoomRange, yZoomRange, (resolution / zoomFactor));

    f->show();
}

void plotExactField(json scenario, Rule rule = Rule{1.0f},
                    std::pair<float, float> xRange = {-4.0, 4.0},
                    std::pair<float, float> yRange = {-4.0, 4.0},
                    std::pair<float, float> xZoomRange = {2.0, 3.0},
                    std::pair<float, float> yZoomRange = {0.0, 1.0}
) {

    Simulation simulation;
    from_json(scenario, simulation);

    auto fieldFunction = [&](double x, double y) {
        auto actorsView = simulation.view<const Position, const Mass>();
        glm::vec3 netForce = {};
        actorsView.each([&](const Position &activePosition, const Mass &activeMass) {
            netForce += (glm::vec3) rule(activePosition, activeMass, Position{x, y, 0.0f});
        });
        return std::log(glm::length(netForce));
    };

    plotFunctionWithZoom(fieldFunction,
                         xRange, yRange,
                         xZoomRange, yZoomRange);

}

void plotApproximateField(json scenario, Rule rule = Rule{1.0f},
                          std::pair<float, float> xRange = {1.0f, 3.0},
                          std::pair<float, float> yRange = {0.0, 2.0},
                          double resolution = 0.05f
) {

    Simulation simulation;
    from_json(scenario, simulation);

    Position samplePosition{(xRange.first + xRange.second) / 2.0f,
                            (yRange.first + yRange.second) / 2.0f,
                            0.0f};

    auto actorsView = simulation.view<const Position, const Mass>();
    glm::vec3 netApproximateAcceleration{};
    QuadrupoleAccelerationSummary netQuadrupoleAcceleration{};
    OctupoleAccelerationSummary netOctupoleAcceleration{};
    HexadecupoleAccelerationSummary netHexadecupoleAcceleration{};
    actorsView.each([&](const Position &activePosition, const Mass &activeMass) {
        netApproximateAcceleration += (glm::vec3) rule(activePosition, activeMass, samplePosition);
        rule(activePosition, activeMass, samplePosition, netQuadrupoleAcceleration);
        rule(activePosition, activeMass, samplePosition, netOctupoleAcceleration);
        rule(activePosition, activeMass, samplePosition, netHexadecupoleAcceleration);
    });

    auto approximateFunction = [&](double x, double y) {
        return netApproximateAcceleration;
    };
    auto quadrupoleApproximateFunction = [&](double x, double y) {
        return netQuadrupoleAcceleration.acceleration().at({x - samplePosition.x, y - samplePosition.y, 0.0f});
    };
    auto octupoleApproximateFunction = [&](double x, double y) {
        return netOctupoleAcceleration.acceleration().at({x - samplePosition.x, y - samplePosition.y, 0.0f});
    };
    auto hexadecupoleApproximateFunction = [&](double x, double y) {
        return netHexadecupoleAcceleration.acceleration().at({x - samplePosition.x, y - samplePosition.y, 0.0f});
    };

    auto f = matplot::figure();
    f->size(800, 800);
    matplot::tiledlayout(4, 2);

    auto ax1 = matplot::nexttile();
    ax1->title("Uniform Component Magnitude (log-scale)");
    ax1->axes_aspect_ratio_auto(false);
    plotFieldLogMagnitudeInRange(ax1, approximateFunction, xRange, yRange, resolution);
    matplot::ellipse(ax1, samplePosition.x, samplePosition.y,
                     resolution, resolution)->color("white");

    auto ax2 = matplot::nexttile();
    ax2->title("Approximate Field Error");
    ax2->axes_aspect_ratio_auto(false);
    plotFieldErrorInRange(ax2, approximateFunction, simulation, rule, xRange, yRange, resolution);
    matplot::ellipse(ax2, samplePosition.x, samplePosition.y,
                     resolution, resolution)->color("white");

    auto ax3 = matplot::nexttile();
    ax3->title("Quadrupole Component Magnitude (log-scale)");
    ax3->axes_aspect_ratio_auto(false);
    plotFieldLogMagnitudeInRange(ax3, quadrupoleApproximateFunction, xRange, yRange, resolution);
    matplot::ellipse(ax3, samplePosition.x, samplePosition.y,
                     resolution, resolution)->color("white");

    auto ax4 = matplot::nexttile();
    ax4->title("Quadrupole Approximate Field Error");
    ax4->axes_aspect_ratio_auto(false);
    plotFieldErrorInRange(ax4, quadrupoleApproximateFunction, simulation, rule, xRange, yRange, resolution);
    matplot::ellipse(ax4, samplePosition.x - resolution / 2, samplePosition.y - resolution / 2,
                     resolution, resolution)->color("white");

    auto ax5 = matplot::nexttile();
    ax5->title("Octupole Component Magnitude (log-scale)");
    ax5->axes_aspect_ratio_auto(false);
    plotFieldLogMagnitudeInRange(ax5, octupoleApproximateFunction, xRange, yRange, resolution);
    matplot::ellipse(ax5, samplePosition.x, samplePosition.y,
                     resolution, resolution)->color("white");

    auto ax6 = matplot::nexttile();
    ax6->title("Octupole Approximate Field Error");
    ax6->axes_aspect_ratio_auto(false);
    plotFieldErrorInRange(ax6, octupoleApproximateFunction, simulation, rule, xRange, yRange, resolution);
    matplot::ellipse(ax6, samplePosition.x - resolution / 2, samplePosition.y - resolution / 2,
                     resolution, resolution)->color("white");

    auto ax7 = matplot::nexttile();
    ax7->title("Hexadecupole Component Magnitude (log-scale)");
    ax7->axes_aspect_ratio_auto(false);
    plotFieldLogMagnitudeInRange(ax7, hexadecupoleApproximateFunction, xRange, yRange, resolution);
    matplot::ellipse(ax7, samplePosition.x, samplePosition.y,
                     resolution, resolution)->color("white");

    auto ax8 = matplot::nexttile();
    ax8->title("Hexadecupole Approximate Field Error");
    ax8->axes_aspect_ratio_auto(false);
    plotFieldErrorInRange(ax8, hexadecupoleApproximateFunction, simulation, rule, xRange, yRange, resolution);
    matplot::ellipse(ax8, samplePosition.x - resolution / 2, samplePosition.y - resolution / 2,
                     resolution, resolution)->color("white");

    f->show();

}

#endif //N_BODY_GRAVITATIONALVIEWINGSHEET_H
