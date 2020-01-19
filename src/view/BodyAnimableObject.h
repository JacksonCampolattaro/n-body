//
// Created by jackcamp on 1/8/20.
//

#ifndef N_BODY_BODYANIMABLEOBJECT_H
#define N_BODY_BODYANIMABLEOBJECT_H

#include "../model/Body.h"
#include "ColoredDrawable.h"

#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/UVSphere.h>

#include <Magnum/Shaders/Phong.h>

#include <Magnum/SceneGraph/TranslationRotationScalingTransformation3D.h>
#include <Magnum/SceneGraph/Animable.h>

#include <Magnum/GlmIntegration/Integration.h>


using namespace Magnum;
using namespace Math::Literals;

//typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Object<SceneGraph::TranslationRotationScalingTransformation3D> Object3D;

class BodyAnimableObject : public Object3D, public SceneGraph::Animable3D {

private:

    Body *body;

public:

    BodyAnimableObject(Object3D *parent, SceneGraph::AnimableGroup3D* group, Body *body);

    void animationStep(Float, Float delta) override;

    void update();

};


#endif //N_BODY_BODYANIMABLEOBJECT_H
