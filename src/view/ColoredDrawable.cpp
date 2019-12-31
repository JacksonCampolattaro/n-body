//
// Created by jackcamp on 12/31/19.
//

#include "ColoredDrawable.h"

void ColoredDrawable::draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) {
    _shader
            .setDiffuseColor(_color)
            .setLightPosition(camera.cameraMatrix().transformPoint({-3.0f, 10.0f, 10.0f}))
            .setTransformationMatrix(transformationMatrix)
            ///.setNormalMatrix(transformationMatrix.normalMatrix())
            .setNormalMatrix(transformationMatrix.rotationNormalized())
            .setProjectionMatrix(camera.projectionMatrix());

    _mesh.draw(_shader);
}
