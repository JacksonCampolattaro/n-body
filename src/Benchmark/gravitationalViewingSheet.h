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

void plotFunctionWithZoom(
        const std::function<float(double, double)> &function,
        const std::pair<float, float> &xRange,
        const std::pair<float, float> &yRange,
        const std::pair<float, float> &xZoomRange,
        const std::pair<float, float> &yZoomRange
) {

    auto f = matplot::figure();
    f->size(1000, 450);
    matplot::tiledlayout(1, 2);

    auto [X, Y] = matplot::meshgrid(matplot::iota(xRange.first, 0.05, xRange.second),
                                    matplot::iota(yRange.first, 0.05, yRange.second));
    auto F = matplot::transform(X, Y, function);
    auto ax1 = matplot::nexttile();
    matplot::imagesc(ax1, xRange.first, xRange.second, yRange.first, yRange.second, F);
    ax1->axes_aspect_ratio_auto(false);
    matplot::rectangle(ax1,
                       xZoomRange.first, yZoomRange.first,
                       xZoomRange.second - xZoomRange.first, yZoomRange.second - yZoomRange.first);

    auto [zoomX, zoomY] = matplot::meshgrid(matplot::iota(xZoomRange.first, 0.01, xZoomRange.second),
                                            matplot::iota(yZoomRange.first, 0.01, yZoomRange.second));
    auto zoomF = matplot::transform(zoomX, zoomY, function);
    auto ax2 = matplot::nexttile();
    matplot::imagesc(ax2, xZoomRange.first, xZoomRange.second, yZoomRange.first, yZoomRange.second, zoomF);
    ax2->axes_aspect_ratio_auto(false);

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

#endif //N_BODY_GRAVITATIONALVIEWINGSHEET_H
