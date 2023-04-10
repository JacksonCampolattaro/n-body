//
// Created by Jackson Campolattaro on 4/8/23.
//

#ifndef N_BODY_GRAVITATIONALVIEWINGSHEET_H
#define N_BODY_GRAVITATIONALVIEWINGSHEET_H

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <glm/gtx/color_space.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <NBody/Physics/Rule.h>
#include <matplot/matplot.h>

using namespace NBody;
using nlohmann::json;

void plotFunctionInRange(
        matplot::axes_handle &ax,
        const std::function<glm::vec3(double, double)> &function,
        const std::pair<float, float> &xRange,
        const std::pair<float, float> &yRange,
        double resolution = 0.05f
) {

    auto [X, Y] = matplot::meshgrid(matplot::iota(xRange.first, resolution, xRange.second),
                                    matplot::iota(yRange.first, resolution, yRange.second));

    float maxMagnitude = 0.0f;
    auto hues = matplot::transform(X, Y, [&](double x, double y) {
        auto v = function(x, y);
        maxMagnitude = std::max(maxMagnitude, glm::length(v));
        return (std::atan2(v.x, v.y) * 180 / M_PI) + 180;
    });
    auto saturations = matplot::transform(X, Y, [&](double x, double y) {
        auto v = function(x, y);
        return std::cbrt(glm::length(v) / maxMagnitude);
    });

    std::tuple<matplot::vector_2d, matplot::vector_2d, matplot::vector_2d> colorsRGB;
    auto &[r, g, b] = colorsRGB;
    r = matplot::transform(hues, saturations, [&](double h, double s) {
        return glm::rgbColor(glm::vec3{h, s, 1.0f}).r * 255;
    });
    g = matplot::transform(hues, saturations, [&](double h, double s) {
        return glm::rgbColor(glm::vec3{h, s, 1.0f}).g * 255;
    });
    b = matplot::transform(hues, saturations, [&](double h, double s) {
        return glm::rgbColor(glm::vec3{h, s, 1.0f}).b * 255;
    });

    ax->axis(false);
    ax->axes_aspect_ratio_auto(false);
    matplot::image(ax, colorsRGB);
}

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
    ax->axes_aspect_ratio_auto(false);
    matplot::imagesc(ax, xRange.first, xRange.second, yRange.first, yRange.second, F);

    auto values = matplot::flatten(F);
    ax->color_box_range(*std::min_element(values.begin(), values.end()),
                        *std::max_element(values.begin(), values.end()));


    // todo: this should be done elsewhere
    //matplot::contour(ax, X, Y, F, std::vector<double>{0.05});

}

void plotFieldMagnitudeInRange(
        matplot::axes_handle &ax,
        const std::function<glm::vec3(double, double)> &function,
        const std::pair<float, float> &xRange,
        const std::pair<float, float> &yRange,
        double resolution = 0.05f
) {
    auto logMagnitudeFunction = [&](double x, double y) {
        return glm::length(function(x, y));
    };
    plotFunctionInRange(ax, logMagnitudeFunction, xRange, yRange, resolution);
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
        return glm::length(netAcceleration - approximateField(x, y)) / glm::length(netAcceleration);
    };

    matplot::colormap(ax, matplot::palette::hot());
    plotFunctionInRange(ax, errorFunction, xRange, yRange, resolution);
    matplot::colorbar(ax);
}

// fixme: this has to be implemented seperately, because matplot::image doesn't handle scaling :(
void plotFunctionWithZoom(
        const std::function<glm::vec3(double, double)> &function,
        const std::pair<float, float> &xRange,
        const std::pair<float, float> &yRange,
        const std::pair<float, float> &xZoomRange,
        const std::pair<float, float> &yZoomRange,
        double resolution = 0.05f
) {

    float zoomFactor = (xRange.second - xRange.first) / (xZoomRange.second - xZoomRange.first);

    auto f = matplot::figure();
    f->size(1000, 450);
    matplot::tiledlayout(1, 2);

    auto ax1 = matplot::nexttile();
    ax1->title("Gravitational Field");
    ax1->axes_aspect_ratio_auto(false);
    plotFunctionInRange(ax1, function, xRange, yRange, resolution);
    ax1->rectangle((xZoomRange.first - xRange.first) / resolution,
                   (yZoomRange.first - yRange.first) / resolution,
                   (xZoomRange.second - xZoomRange.first) / resolution,
                   (yZoomRange.second - yZoomRange.first) / resolution)->color("black");

    auto ax2 = matplot::nexttile();
    ax2->title("Local Gravitational Field");
    ax2->axes_aspect_ratio_auto(false);
    plotFunctionInRange(ax2, function, xZoomRange, yZoomRange, (resolution / zoomFactor));

    f->show();
}

void plotFunctionWithZoom(
        const std::function<float(double, double)> &function,
        const std::pair<float, float> &xRange,
        const std::pair<float, float> &yRange,
        const std::pair<float, float> &xZoomRange,
        const std::pair<float, float> &yZoomRange,
        double resolution = 0.1f
) {

    float zoomFactor = (xRange.second - xRange.first) / (xZoomRange.second - xZoomRange.first);

    auto f = matplot::figure();
    f->size(1000, 450);
    matplot::tiledlayout(1, 2);

    auto ax1 = matplot::nexttile();
    ax1->title("Gravitational Field Magnitude (log-scale)");
    plotFunctionInRange(ax1, function, xRange, yRange, resolution);
    matplot::rectangle(ax1, xZoomRange.first, yZoomRange.first,
                       xZoomRange.second - xZoomRange.first, yZoomRange.second - yZoomRange.first);

    auto ax2 = matplot::nexttile();
    ax2->title("Local Field Magnitude Across Selected Region (log-scale)");
    plotFunctionInRange(ax2, function, xZoomRange, yZoomRange, (resolution / zoomFactor));

    f->show();
}

void plotExactField(json scenario, Rule rule = Rule{1.0f},
                    std::pair<float, float> xRange = {-10.0, 110.0},
                    std::pair<float, float> yRange = {-10.0, 110.0},
                    std::pair<float, float> xZoomRange = {1.0, 2.0},
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
        return netForce;
    };

    plotFunctionWithZoom(fieldFunction, xRange, yRange, xZoomRange, yZoomRange);

//    auto f = matplot::figure();
//    auto ax = matplot::axes();
//    plotFunctionInRange(ax, fieldFunction, xRange, yRange, 0.5f);
//    f->show();

}

void plotFieldApproximations(json scenario, Rule rule = Rule{1.0f},
                             std::pair<float, float> xRange = {1.0f, 1.5f},
                             std::pair<float, float> yRange = {0.0, 0.5f},
                             double resolution = 0.01f
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
    TriacontadyupoleAccelerationSummary netTriacontadyupoleAcceleration{};
    actorsView.each([&](const Position &activePosition, const Mass &activeMass) {
        netApproximateAcceleration += (glm::vec3) rule(activePosition, activeMass, samplePosition);
        rule(activePosition, activeMass, samplePosition, netQuadrupoleAcceleration);
        rule(activePosition, activeMass, samplePosition, netOctupoleAcceleration);
        rule(activePosition, activeMass, samplePosition, netHexadecupoleAcceleration);
        rule(activePosition, activeMass, samplePosition, netTriacontadyupoleAcceleration);
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
    auto triacontadyupoleApproximateFunction = [&](double x, double y) {
        return netHexadecupoleAcceleration.acceleration().at({x - samplePosition.x, y - samplePosition.y, 0.0f});
    };

    auto uniformComponentFunction = [&](double x, double y) {
        return netApproximateAcceleration;
    };
    auto quadrupoleComponentFunction = [&](double x, double y) {
        return quadrupoleApproximateFunction(x, y) - approximateFunction(x, y);
    };
    auto octupoleComponentFunction = [&](double x, double y) {
        return octupoleApproximateFunction(x, y) - quadrupoleApproximateFunction(x, y);
    };
    auto hexadecupoleComponentFunction = [&](double x, double y) {
        return hexadecupoleApproximateFunction(x, y) - octupoleApproximateFunction(x, y);
    };
    auto triacontadyupoleComponentFunction = [&](double x, double y) {
        return triacontadyupoleApproximateFunction(x, y) - hexadecupoleApproximateFunction(x, y);
    };

    auto f = matplot::figure();
    f->size(550, 825);
    matplot::tiledlayout(4, 2);
    std::vector<std::vector<matplot::axes_handle>> axes{
            {matplot::nexttile(), matplot::nexttile()},
            {matplot::nexttile(), matplot::nexttile()},
            {matplot::nexttile(), matplot::nexttile()},
            {matplot::nexttile(), matplot::nexttile()},
            //{matplot::nexttile(), matplot::nexttile()}
    };

    axes[0][0]->title("Uniform Component Magnitude");
    plotFieldMagnitudeInRange(axes[0][0], uniformComponentFunction, xRange, yRange, resolution);
    axes[0][1]->title("Approximate Field Error");
    plotFieldErrorInRange(axes[0][1], approximateFunction, simulation, rule, xRange, yRange, resolution);

    axes[1][0]->title("Quadrupole Component Magnitude");
    plotFieldMagnitudeInRange(axes[1][0], quadrupoleComponentFunction, xRange, yRange, resolution);
    axes[1][1]->title("Quadrupole Approximate Field Error");
    plotFieldErrorInRange(axes[1][1], quadrupoleApproximateFunction, simulation, rule, xRange, yRange, resolution);

    axes[2][0]->title("Octupole Component Magnitude");
    plotFieldMagnitudeInRange(axes[2][0], octupoleComponentFunction, xRange, yRange, resolution);
    axes[2][1]->title("Octupole Approximate Field Error");
    plotFieldErrorInRange(axes[2][1], octupoleApproximateFunction, simulation, rule, xRange, yRange, resolution);

    axes[3][0]->title("Hexadecupole Component Magnitude");
    plotFieldMagnitudeInRange(axes[3][0], hexadecupoleComponentFunction, xRange, yRange, resolution);
    axes[3][1]->title("Hexadecupole Approximate Field Error");
    plotFieldErrorInRange(axes[3][1], hexadecupoleApproximateFunction, simulation, rule, xRange, yRange, resolution);

    //    axes[4][0]->title("Triacontadyupole Component Magnitude");
    //    plotFieldMagnitudeInRange(axes[4][0], triacontadyupoleComponentFunction, xRange, yRange, resolution);
    //    axes[4][1]->title("Triacontadyupole Approximate Field Error");
    //    plotFieldErrorInRange(axes[4][1], triacontadyupoleApproximateFunction, simulation, rule, xRange, yRange, resolution);

    // Make sure all errors are shown in the same range
    axes[1][1]->color_box_range(axes[0][1]->color_box_range());
    axes[2][1]->color_box_range(axes[0][1]->color_box_range());
    axes[3][1]->color_box_range(axes[0][1]->color_box_range());
    //axes[4][1]->color_box_range(axes[0][1]->color_box_range());

    for (auto &ax: f->children()) {
        matplot::ellipse(ax, samplePosition.x, samplePosition.y, resolution, resolution)->color("white");
        ax->axes_aspect_ratio_auto(false);
    }

    f->show();

}

void plotMomentApproximations(json scenario, Rule rule = Rule{1.0f},
                              std::pair<float, float> xRange = {-2.0f, 2.0f},
                              std::pair<float, float> yRange = {-2.0, 2.0f},
                              double resolution = 0.1f
) {


    Simulation simulation;
    from_json(scenario, simulation);


    CenterOfMassSummary centerOfMassSummary{};
    QuadrupoleMassSummary quadrupoleMassSummary{};
    OctupoleMassSummary octupoleMassSummary{};
    HexadecupoleMassSummary hexadecupoleMassSummary{};

    auto actorsView = simulation.view<const Position, const Mass>();
    std::vector<Entity> a{actorsView.begin(), actorsView.end()};
    std::span<Entity> actors{a.begin(), a.end()};
    centerOfMassSummary.summarize(actors, actorsView);
    quadrupoleMassSummary.summarize(actors, actorsView);
    octupoleMassSummary.summarize(actors, actorsView);
    hexadecupoleMassSummary.summarize(actors, actorsView);

    auto centerOfMassApproximateFunction = [&](double x, double y) {
        return rule(centerOfMassSummary.centerOfMass(), centerOfMassSummary, {x, y, 0.0f});
    };
    auto quadrupoleApproximateFunction = [&](double x, double y) {
        return rule(quadrupoleMassSummary.centerOfMass(), quadrupoleMassSummary, {x, y, 0.0f});
    };
    auto octupoleApproximateFunction = [&](double x, double y) {
        return rule(octupoleMassSummary.centerOfMass(), octupoleMassSummary, {x, y, 0.0f});
    };
    auto hexadecupoleApproximateFunction = [&](double x, double y) {
        return rule(hexadecupoleMassSummary.centerOfMass(), hexadecupoleMassSummary, {x, y, 0.0f});
    };

    auto centerOfMassComponentFunction = [&](double x, double y) {
        return centerOfMassApproximateFunction(x, y);
    };
    auto quadrupoleComponentFunction = [&](double x, double y) {
        return quadrupoleApproximateFunction(x, y) - centerOfMassApproximateFunction(x, y);
    };
    auto octupoleComponentFunction = [&](double x, double y) {
        return octupoleApproximateFunction(x, y) - quadrupoleApproximateFunction(x, y);
    };
    auto hexadecupoleComponentFunction = [&](double x, double y) {
        return hexadecupoleApproximateFunction(x, y) - octupoleApproximateFunction(x, y);
    };

    auto f = matplot::figure();
    f->size(550, 825);
    matplot::tiledlayout(4, 2);
    std::vector<std::vector<matplot::axes_handle>> axes{
            {matplot::nexttile(), matplot::nexttile()},
            {matplot::nexttile(), matplot::nexttile()},
            {matplot::nexttile(), matplot::nexttile()},
            {matplot::nexttile(), matplot::nexttile()},
    };

    axes[0][0]->title("Uniform Component Magnitude");
    plotFieldMagnitudeInRange(axes[0][0], centerOfMassComponentFunction, xRange, yRange, resolution);
    axes[0][1]->title("Approximate Field Error");
    plotFieldErrorInRange(axes[0][1], centerOfMassApproximateFunction, simulation, rule, xRange, yRange, resolution);

    axes[1][0]->title("Quadrupole Component Magnitude");
    plotFieldMagnitudeInRange(axes[1][0], quadrupoleComponentFunction, xRange, yRange, resolution);
    axes[1][1]->title("Quadrupole Approximate Field Error");
    plotFieldErrorInRange(axes[1][1], quadrupoleApproximateFunction, simulation, rule, xRange, yRange, resolution);

    axes[2][0]->title("Octupole Component Magnitude");
    plotFieldMagnitudeInRange(axes[2][0], octupoleComponentFunction, xRange, yRange, resolution);
    axes[2][1]->title("Octupole Approximate Field Error");
    plotFieldErrorInRange(axes[2][1], octupoleApproximateFunction, simulation, rule, xRange, yRange, resolution);

    axes[3][0]->title("Hexadecupole Component Magnitude");
    plotFieldMagnitudeInRange(axes[3][0], hexadecupoleComponentFunction, xRange, yRange, resolution);
    axes[3][1]->title("Hexadecupole Approximate Field Error");
    plotFieldErrorInRange(axes[3][1], hexadecupoleApproximateFunction, simulation, rule, xRange, yRange, resolution);

    // Make sure all errors are shown in the same range
    axes[1][1]->color_box_range(axes[0][1]->color_box_range());
    axes[2][1]->color_box_range(axes[0][1]->color_box_range());
    axes[3][1]->color_box_range(axes[0][1]->color_box_range());

    for (auto &ax: f->children()) {
        matplot::ellipse(ax, centerOfMassSummary.centerOfMass().x, centerOfMassSummary.centerOfMass().y,
                         resolution, resolution)->color("white");
        ax->axes_aspect_ratio_auto(false);
    }

    f->show();

}

#endif //N_BODY_GRAVITATIONALVIEWINGSHEET_H
