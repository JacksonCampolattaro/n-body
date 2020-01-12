//
// Created by jackcamp on 1/8/20.
//

#ifndef N_BODY_BODYDRAWABLEOBJECT_H
#define N_BODY_BODYDRAWABLEOBJECT_H

#include "../model/Body.h"
#include "ColoredDrawable.h"

#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/UVSphere.h>

#include <Magnum/Shaders/Phong.h>

#include <Magnum/SceneGraph/MatrixTransformation3D.h>


using namespace Magnum;
using namespace Math::Literals;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;

class BodyDrawableObject : public Object3D {

private:

    Body *body;

public:

    BodyDrawableObject(Object3D *parent, Body *body);

    void update();

};


#endif //N_BODY_BODYDRAWABLEOBJECT_H
