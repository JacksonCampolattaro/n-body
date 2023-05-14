//
// Created by Jackson Campolattaro on 1/15/23.
//

#ifndef N_BODY_SOLVERRENDERER_H
#define N_BODY_SOLVERRENDERER_H

#include <Corrade/Containers/GrowableArray.h>

#include <Magnum/GL/Renderer.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/MeshVisualizerGL.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/GL/Mesh.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/MultiSolver.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/DualTreeSolver.h>
#include <NBody/Simulation/Solvers/DualTraversalSolver.h>

#include "../Renderer.h"

#include <queue>

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    struct BoxInstanceData {
        Matrix4 transformationMatrix;
        Color4 color;
    };

    template<RuleType Rule = Gravity>
    class SolverRenderer : public Renderer {
    protected:

        const MultiSolver<Rule> &_solver;
        bool _enabled = false;

        mutable sigc::signal<void()> _signal_changed;

    public:

        SolverRenderer(const MultiSolver<Rule> &solver) :
                Renderer(),
                _solver(solver) {}

        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix) override {

            if (!_enabled) return;

            const auto &solver = _solver.get();
            if (typeid(solver) == typeid(NBody::BarnesHutSolver<Gravity>))
                draw(transformationMatrix, projectionMatrix,
                     dynamic_cast<const NBody::BarnesHutSolver<Gravity> &>(solver));
            else if (typeid(solver) == typeid(NBody::LinearBVHSolver<Gravity>))
                draw(transformationMatrix, projectionMatrix,
                     dynamic_cast<const NBody::LinearBVHSolver<Gravity> &>(solver));
            else if (typeid(solver) == typeid(NBody::MVDRSolver<Gravity>))
                draw(transformationMatrix, projectionMatrix,
                     dynamic_cast<const NBody::MVDRSolver<Gravity> &>(solver));
            else if (typeid(solver) == typeid(NBody::OctreeDualTraversalSolver<Gravity>))
                draw(transformationMatrix, projectionMatrix,
                     dynamic_cast<const NBody::OctreeDualTraversalSolver<Gravity> &>(solver));
        }

        sigc::signal<void()> &signal_changed() override { return _signal_changed; };

        void setEnabled(bool enabled) {
            _enabled = enabled;
            _signal_changed.emit();
        }

        const bool &enabled() const { return _enabled; }

    private:
        // todo: these should all be generic, with support for any ActiveTree, DualTree, etc.

        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix,
                  const BarnesHutSolver<Gravity> &solver) {
            draw(transformationMatrix, projectionMatrix, solver.tree().root(),
                 0xFFFFFFAA_rgbaf);
        }

        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix,
                  const LinearBVHSolver<Gravity> &solver) {
            draw(transformationMatrix, projectionMatrix, solver.tree().root(),
                 0xFFFFFFAA_rgbaf);
        }

        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix,
                  const MVDRSolver<Gravity> &solver) {
            draw(transformationMatrix, projectionMatrix, solver.activeTree().root(), 0xFF000055_rgbaf);
            draw(transformationMatrix, projectionMatrix, solver.passiveTree().root(), 0x0000FF55_rgbaf);
        }

        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix,
                  const OctreeDualTraversalSolver<Gravity> &solver) {
            draw(transformationMatrix, projectionMatrix, solver.tree().root(),
                 0xFFFFFFAA_rgbaf);
        }

        template<typename TreeNode>
        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix,
                  const TreeNode &root,
                  const Color4 &color) {

            auto shader = Shaders::FlatGL3D{Shaders::FlatGL3D::Flag::VertexColor |
                                            Shaders::FlatGL3D::Flag::InstancedTransformation};
            auto mesh = MeshTools::compile(Primitives::cubeWireframe());
            GL::Buffer boxInstanceBuffer{};
            Containers::Array<BoxInstanceData> boxInstanceData;
            mesh.addVertexBufferInstanced(boxInstanceBuffer, 1, 0,
                                          Shaders::FlatGL3D::TransformationMatrix{},
                                          Shaders::FlatGL3D::Color4{});

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

                Containers::arrayAppend(boxInstanceData, InPlaceInit,
                                        individualTransformationMatrix, color);
            }

            boxInstanceBuffer.setData(boxInstanceData, GL::BufferUsage::DynamicDraw);
            mesh.setInstanceCount((int) boxInstanceData.size());

            shader
                    .setTransformationProjectionMatrix(projectionMatrix)
                    .draw(mesh);

        }

    };

}

#endif //N_BODY_SOLVERRENDERER_H
