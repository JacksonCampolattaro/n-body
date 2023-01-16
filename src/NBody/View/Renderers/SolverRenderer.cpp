//
// Created by Jackson Campolattaro on 1/15/23.
//

#include "SolverRenderer.h"

#include <Magnum/Primitives/Cube.h>

void NBody::SolverRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix,
                                 const NBody::Simulation &simulation, const NBody::Solver &solver) {


    if (typeid(solver) == typeid(NBody::BarnesHutSolver))
        draw(transformationMatrix, projectionMatrix, simulation, dynamic_cast<const NBody::BarnesHutSolver &>(solver));
    else if (typeid(solver) == typeid(NBody::LinearBVHSolver))
        draw(transformationMatrix, projectionMatrix, simulation, dynamic_cast<const NBody::LinearBVHSolver &>(solver));
}

void NBody::SolverRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix,
                                 const NBody::Simulation &simulation, const NBody::BarnesHutSolver &solver) {
    draw(transformationMatrix, projectionMatrix, solver.tree().root());
}

void NBody::SolverRenderer::draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix,
                                 const NBody::Simulation &simulation, const NBody::LinearBVHSolver &solver) {
    draw(transformationMatrix, projectionMatrix, solver.tree().root());
}
