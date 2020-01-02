//
// Created by jackcamp on 12/31/19.
//

#include "ColoredDrawable.h"

ColoredDrawable::ColoredDrawable(Object3D &object, SceneGraph::DrawableGroup3D &group, GL::Mesh &mesh,
                                 const Color4 &color,
                                 Shaders::Phong &shader) :

        SceneGraph::Drawable3D{object, &group},
        _shader(shader),
        _mesh(mesh),
        _color{color} {


}

void ColoredDrawable::draw(const Matrix4 &transformation, SceneGraph::Camera3D &camera) {
    _shader
            .setLightPosition({7.0f, 5.0f, 2.5f})
            .setLightColor(Color3{1.0f})
            .setDiffuseColor(_color)
            .setAmbientColor(Color3::fromHsv({_color.hue(), 1.0f, 0.3f}))
            .setTransformationMatrix(transformation)
            .setProjectionMatrix(camera.projectionMatrix())
            .setNormalMatrix(transformation.rotationNormalized());


    _mesh.draw(_shader);
}
