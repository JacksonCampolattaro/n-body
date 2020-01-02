//
// Created by jackcamp on 12/31/19.
//

#include "ColoredDrawable.h"

ColoredDrawable::ColoredDrawable(Object3D &object, SceneGraph::DrawableGroup3D &group, GL::Mesh &mesh,
                                 Shaders::Phong &shader,
                                 const Color4 &color, const Matrix4 &modifier) :

        SceneGraph::Drawable3D{object, &group},
        shader(shader),
        mesh(mesh),
        color{color},
        modifier(modifier)
        {}

void ColoredDrawable::draw(const Matrix4 &transformation, SceneGraph::Camera3D &camera) {
    shader
            .setDiffuseColor(color)
            ///.setAmbientColor(Color3::fromHsv({color.hue(), 1.0f, 0.3f}))
            .setAmbientColor(color / 3)
            .setTransformationMatrix(transformation * modifier)
            .setProjectionMatrix(camera.projectionMatrix())
            .setNormalMatrix(transformation.rotationNormalized());


    mesh.draw(shader);
}