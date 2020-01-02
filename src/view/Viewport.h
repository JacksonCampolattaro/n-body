//
// Created by jackcamp on 12/26/19.
//

#ifndef N_BODY_VIEWPORT_H
#define N_BODY_VIEWPORT_H

#include "../model/tools/BodyList.h"
#include "../model/PhysicsContext.h"
#include "../Config.h"

#include <fstream>

#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>

#include <CLI/CLI.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Primitives/UVSphere.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData3D.h>

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>

using std::cout;
using std::endl;
using std::string;
using std::make_shared;

using namespace Magnum;
using namespace Math::Literals;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

class Viewport : public Magnum::Platform::Application {

public:

    explicit Viewport(const Arguments& arguments);

private:

    void drawEvent() override;


    Shaders::Phong shader{NoCreate}; // Unified shader for all spheres?
    GL::Mesh sphereMesh{NoCreate}; // Perhaps they can share a Mesh too?


    // Switching to a SceneGraph solution
    Scene3D scene;
    SceneGraph::Camera3D* camera;
    SceneGraph::DrawableGroup3D drawables;

    Object3D manipulator, cameraObject;
};


#endif //N_BODY_VIEWPORT_H
