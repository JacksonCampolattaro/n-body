//
// Created by jackcamp on 12/31/19.
//

#ifndef N_BODY_COLOREDDRAWABLE_H
#define N_BODY_COLOREDDRAWABLE_H

#include <Corrade/Containers/Array.h>
#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Arguments.h>
#include <Magnum/ImageView.h>
#include <Magnum/Mesh.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/TranslationRotationScalingTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Trade/PhongMaterialData.h>
#include <Magnum/Trade/SceneData.h>
#include <Magnum/Trade/TextureData.h>

using namespace Magnum;

//typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
//typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

typedef SceneGraph::Object<SceneGraph::TranslationRotationScalingTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::TranslationRotationScalingTransformation3D> Scene3D;

static Color4 DEFAULT_COLOR = Color3::fromHsv({Math::Deg<float>(35.0), 1.0f, 1.0f});
static Matrix4 DEFAULT_MODIFIER = {};


class ColoredDrawable : public SceneGraph::Drawable3D {
public:

    explicit ColoredDrawable(Object3D &object, SceneGraph::DrawableGroup3D &group, GL::Mesh &mesh,
                             Shaders::Phong &shader,
                             const Color4 &color = DEFAULT_COLOR, const Matrix4 &modifier = DEFAULT_MODIFIER);

private:

    void draw(const Matrix4 &transformation, SceneGraph::Camera3D &camera) override;

    Shaders::Phong &shader;
    GL::Mesh &mesh;
    Color4 color;
    Matrix4 modifier;

};


#endif //N_BODY_COLOREDDRAWABLE_H
