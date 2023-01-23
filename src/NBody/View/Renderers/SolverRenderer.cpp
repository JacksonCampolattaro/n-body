//
// Created by Jackson Campolattaro on 1/15/23.
//

#include "SolverRenderer.h"

#include <Magnum/Primitives/Cube.h>

void NBody::SolverRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) {

    if (!_enabled) return;

    const auto &solver = _solver.get();
    if (typeid(solver) == typeid(NBody::BarnesHutSolver))
        draw(transformationMatrix, projectionMatrix, dynamic_cast<const NBody::BarnesHutSolver &>(solver));
    else if (typeid(solver) == typeid(NBody::LinearBVHSolver))
        draw(transformationMatrix, projectionMatrix, dynamic_cast<const NBody::LinearBVHSolver &>(solver));
    else if (typeid(solver) == typeid(NBody::MVDRSolver))
        draw(transformationMatrix, projectionMatrix, dynamic_cast<const NBody::MVDRSolver &>(solver));
}

void NBody::SolverRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix,
                                 const NBody::BarnesHutSolver &solver) {
    draw(transformationMatrix, projectionMatrix, solver.tree().root(), 0x0A0A0AAA_rgbaf);
}

void NBody::SolverRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix,
                                 const NBody::LinearBVHSolver &solver) {
    draw(transformationMatrix, projectionMatrix, solver.tree().root(), 0x0A0A0AAA_rgbaf);
}

void NBody::SolverRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix,
                                 const NBody::MVDRSolver &solver) {
    draw(transformationMatrix, projectionMatrix, solver.activeTree().root(), 0xFF000055_rgbaf);
    draw(transformationMatrix, projectionMatrix, solver.passiveTree().root(), 0x0000FF55_rgbaf);
}
