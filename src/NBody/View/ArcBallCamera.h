#ifndef Magnum_Examples_ArcBallCamera_h
#define Magnum_Examples_ArcBallCamera_h
/*
    This file is part of Magnum.

    Original authors — credit is appreciated but not required:

        2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021
             — Vladimír Vondruš <mosra@centrum.cz>
        2020 — Nghia Truong <nghiatruong.vn@gmail.com>

    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or distribute
    this software, either in source code form or as a compiled binary, for any
    purpose, commercial or non-commercial, and by any means.

    In jurisdictions that recognize copyright laws, the author or authors of
    this software dedicate any and all copyright interest in the software to
    the public domain. We make this dedication for the benefit of the public
    at large and to the detriment of our heirs and successors. We intend this
    dedication to be an overt act of relinquishment in perpetuity of all
    present and future rights to this software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/AbstractTranslationRotation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>

#include <Magnum/GL/Mesh.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Math/Color.h>

#include "ArcBall.h"
#include "Camera.h"

#include <iostream>

namespace Magnum::Examples {

    using namespace Math::Literals;

    /* Arcball camera implementation integrated into the SceneGraph */
    class ArcBallCamera : public ArcBall {
    public:
        ArcBallCamera(
                const Vector3 &cameraPosition, const Vector3 &viewCenter,
                const Vector3 &upDir, Deg fov, const Vector2i &windowSize,
                const Vector2i &viewportSize) :
                ArcBall{cameraPosition, viewCenter, upDir, fov, windowSize} {

            _camera._projection = Matrix4::perspectiveProjection(
                    fov, Vector2{windowSize}.aspectRatio(), 0.01f, 100.0f);
            _camera._transformation = transformationMatrix();
//                    Matrix4::translation(transformation().translation()) *
//                    Matrix4::from((transformationMatrix()).rotationScaling().inverted(), {0, 0, 0});
        }

        /* Update screen and viewport size after the window has been resized */
        void reshape(const Vector2i &windowSize, const Vector2i &viewportSize) {
            _windowSize = windowSize;

            _camera._projection = Matrix4::perspectiveProjection(35.0_degf,
                                                                 Vector2{_windowSize}.aspectRatio(),
                                                                 0.01f,100.0f);
        }

        /* Update the SceneGraph camera if arcball has been changed */
        bool update() {
            /* call the internal update */
            if (!updateTransformation()) return false;

            _camera._transformation = Matrix4::translation(transformation().translation());
            return true;
        }

        void draw() {

            auto transformation =
                    _camera._transformation
                    * Matrix4::rotationX(30.0_degf) * Matrix4::rotationY(40.0_degf);

            auto mesh = MeshTools::compile(Primitives::cubeSolid());
            auto color = Color3::fromHsv({35.0_degf, 1.0, 1.0});

            auto shader = Shaders::PhongGL{{}, 1};
            shader
                    .setLightPositions({{24.0f, 50.0f, -50, 0.0f}})
                    .setDiffuseColor(color)
                    .setAmbientColor(Color3::fromHsv({color.hue(), 1.0f, 0.3f}))
                    .setTransformationMatrix(transformation)
                    .setNormalMatrix(transformation.normalMatrix())
                    .setProjectionMatrix(_camera._projection)
                    .draw(mesh);
        };

        NBody::Camera &camera() { return _camera; }

    private:

        NBody::Camera _camera;
    };

}

#endif
