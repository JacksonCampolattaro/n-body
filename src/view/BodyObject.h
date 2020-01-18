//
// Created by jackcamp on 1/8/20.
//

#ifndef N_BODY_BODYOBJECT_H
#define N_BODY_BODYOBJECT_H

#include "../model/Body.h"
#include "ColoredDrawable.h"

#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/UVSphere.h>

#include <Magnum/Shaders/Phong.h>

#include <Magnum/SceneGraph/TranslationRotationScalingTransformation3D.h>

#include <Magnum/GlmIntegration/Integration.h>


using namespace Magnum;
using namespace Math::Literals;

//typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Object<SceneGraph::TranslationRotationScalingTransformation3D> Object3D;

class BodyObject : public Object3D {

private:

    Body *body;

public:

    BodyObject(Object3D *parent, Body *body);

    void update();

};


#endif //N_BODY_BODYOBJECT_H
