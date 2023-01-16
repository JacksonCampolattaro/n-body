//
// Created by Jackson Campolattaro on 1/15/23.
//

#ifndef N_BODY_SOLVERRENDERER_H
#define N_BODY_SOLVERRENDERER_H

#include <Magnum/GL/Renderer.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/MeshVisualizerGL.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/GL/Mesh.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/MultiSolver.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>

#include "../Renderer.h"

#include <queue>

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    class SolverRenderer : public Renderer {
    protected:

        const MultiSolver &_solver;

    public:

        SolverRenderer(const MultiSolver &solver) : _solver(solver) {}

        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix) override;

        sigc::signal<void()> &signal_changed() override { return _solver.signal_finished(); };

    private:

        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix,
                  const BarnesHutSolver &solver);

        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix,
                  const LinearBVHSolver &solver);

        template<typename TreeNode>
        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix,
                  const TreeNode &root) {

            auto shader = Shaders::PhongGL{Shaders::PhongGL::Flag::NoSpecular};
            auto mesh = MeshTools::compile(Primitives::cubeWireframe());

            std::queue<const TreeNode *> nodes;
            nodes.emplace(&root);
            while (!nodes.empty()) {
                const TreeNode &node = *nodes.front();
                nodes.pop();
                if (!node.isLeaf()) {
                    for (const TreeNode &child: node.children())
                        nodes.emplace(&child);
                }

                auto translation = node.boundingBox().center();
                auto scaling = node.boundingBox().dimensions() / 2.0f;

                auto individualTransformationMatrix =
                        transformationMatrix *
                        Matrix4::translation({translation.x, translation.y, translation.z}) *
                        Matrix4::scaling({scaling.x, scaling.y, scaling.z});

                shader
                        .setProjectionMatrix(projectionMatrix)
                        .setTransformationMatrix(individualTransformationMatrix)
                        .draw(mesh);
            }

        }

    };

}

#endif //N_BODY_SOLVERRENDERER_H
