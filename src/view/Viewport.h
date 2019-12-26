//
// Created by jackcamp on 12/26/19.
//

#ifndef N_BODY_VIEWPORT_H
#define N_BODY_VIEWPORT_H

#include "../model/tools/BodyList.h"

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData3D.h>

class Viewport : public Magnum::Platform::Application {

public:

    explicit Viewport(BodyList *bodies);

private:

    void drawEvent() override;

    Magnum::GL::Mesh _mesh;
    Magnum::Shaders::Phong _shader;

    Magnum::Matrix4 _transformation, _projection;
    Magnum::Color3 _color;
};


#endif //N_BODY_VIEWPORT_H
