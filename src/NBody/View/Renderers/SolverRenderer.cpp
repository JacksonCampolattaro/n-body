//
// Created by Jackson Campolattaro on 1/15/23.
//

#include "SolverRenderer.h"

#include <Magnum/Primitives/Cube.h>

void NBody::SolverRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) {
    const auto &solver = _solver.get();
    if (typeid(solver) == typeid(NBody::BarnesHutSolver))
        draw(transformationMatrix, projectionMatrix, dynamic_cast<const NBody::BarnesHutSolver &>(solver));
    else if (typeid(solver) == typeid(NBody::LinearBVHSolver))
        draw(transformationMatrix, projectionMatrix, dynamic_cast<const NBody::LinearBVHSolver &>(solver));
}

void NBody::SolverRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix,
                                 const NBody::BarnesHutSolver &solver) {
    draw(transformationMatrix, projectionMatrix, solver.tree().root());
}

void NBody::SolverRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix,
                                 const NBody::LinearBVHSolver &solver) {
    draw(transformationMatrix, projectionMatrix, solver.tree().root());
}
