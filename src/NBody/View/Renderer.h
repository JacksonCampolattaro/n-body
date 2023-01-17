//
// Created by Jackson Campolattaro on 7/24/22.
//

#ifndef N_BODY_RENDERER_H
#define N_BODY_RENDERER_H

#include <Magnum/GL/Renderer.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Math/Color.h>

#include <NBody/Simulation/Simulation.h>

namespace NBody {

    class Renderer {
    public:

        virtual void draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) = 0;

        virtual sigc::signal<void()> &signal_changed() = 0;

    };

    class SimulationRenderer : public Renderer {
    protected:

        const NBody::Simulation &_simulation;

    public:

        explicit SimulationRenderer(const NBody::Simulation &simulation) : _simulation(simulation) {}

        sigc::signal<void()> &signal_changed() override { return _simulation.signal_changed; };
    };

    using RendererList = std::vector<std::reference_wrapper<Renderer>>;
}

#endif //N_BODY_RENDERER_H
