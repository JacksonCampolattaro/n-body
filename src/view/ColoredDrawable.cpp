//
// Created by jackcamp on 12/31/19.
//

#include "ColoredDrawable.h"

void ColoredDrawable::draw(const Matrix4 &transformation, SceneGraph::Camera3D &camera) {
    _shader.setDiffuseColor(_color)
            .setTransformationMatrix(transformation)
            .setProjectionMatrix(camera.projectionMatrix())
            .setNormalMatrix(transformation.rotationNormalized());

    _mesh.draw(_shader);
}
