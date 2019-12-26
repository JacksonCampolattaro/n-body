/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "AbstractImporter.h"

#include <Corrade/Containers/Array.h>
#include <Corrade/Containers/EnumSet.hpp>
#include <Corrade/Containers/Optional.h>
#include <Corrade/Utility/Assert.h>
#include <Corrade/Utility/DebugStl.h>
#include <Corrade/Utility/Directory.h>

#include "Magnum/FileCallback.h"
#include "Magnum/Trade/AbstractMaterialData.h"
#include "Magnum/Trade/AnimationData.h"
#include "Magnum/Trade/CameraData.h"
#include "Magnum/Trade/ImageData.h"
#include "Magnum/Trade/LightData.h"
#include "Magnum/Trade/MeshData2D.h"
#include "Magnum/Trade/MeshData3D.h"
#include "Magnum/Trade/ObjectData2D.h"
#include "Magnum/Trade/ObjectData3D.h"
#include "Magnum/Trade/SceneData.h"
#include "Magnum/Trade/TextureData.h"

#ifndef CORRADE_PLUGINMANAGER_NO_DYNAMIC_PLUGIN_SUPPORT
#include "Magnum/Trade/configure.h"
#endif

namespace Magnum { namespace Trade {

std::string AbstractImporter::pluginInterface() {
    return "cz.mosra.magnum.Trade.AbstractImporter/0.3";
}

#ifndef CORRADE_PLUGINMANAGER_NO_DYNAMIC_PLUGIN_SUPPORT
std::vector<std::string> AbstractImporter::pluginSearchPaths() {
    return {
        #ifdef CORRADE_IS_DEBUG_BUILD
        #ifndef MAGNUM_BUILD_STATIC
        Utility::Directory::join(Utility::Directory::path(Utility::Directory::libraryLocation(&pluginInterface)), "magnum-d/importers"),
        #else
        #ifndef MAGNUM_TARGET_WINDOWS
        /* On Windows, the plugin DLLs are next to the executable, so the one
           below works. Elsewhere the plugins are in the lib dir instead */
        "../lib/magnum-d/importers",
        #endif
        "magnum-d/importers",
        #endif
        Utility::Directory::join(MAGNUM_PLUGINS_DEBUG_DIR, "importers")
        #ifdef CORRADE_TARGET_WINDOWS
        #endif
        #else
        #ifndef MAGNUM_BUILD_STATIC
        Utility::Directory::join(Utility::Directory::path(Utility::Directory::libraryLocation(&pluginInterface)), "magnum/importers"),
        #else
        #ifndef MAGNUM_TARGET_WINDOWS
        "../lib/magnum/importers",
        #endif
        "magnum/importers",
        #endif
        Utility::Directory::join(MAGNUM_PLUGINS_DIR, "importers")
        #endif
    };
}
#endif

AbstractImporter::AbstractImporter() = default;

AbstractImporter::AbstractImporter(PluginManager::Manager<AbstractImporter>& manager): PluginManager::AbstractManagingPlugin<AbstractImporter>{manager} {}

AbstractImporter::AbstractImporter(PluginManager::AbstractManager& manager, const std::string& plugin): PluginManager::AbstractManagingPlugin<AbstractImporter>{manager, plugin} {}

void AbstractImporter::setFileCallback(Containers::Optional<Containers::ArrayView<const char>>(*callback)(const std::string&, InputFileCallbackPolicy, void*), void* const userData) {
    CORRADE_ASSERT(!isOpened(), "Trade::AbstractImporter::setFileCallback(): can't be set while a file is opened", );
    CORRADE_ASSERT(features() & (Feature::FileCallback|Feature::OpenData), "Trade::AbstractImporter::setFileCallback(): importer supports neither loading from data nor via callbacks, callbacks can't be used", );

    _fileCallback = callback;
    _fileCallbackUserData = userData;
    doSetFileCallback(callback, userData);
}

void AbstractImporter::doSetFileCallback(Containers::Optional<Containers::ArrayView<const char>>(*)(const std::string&, InputFileCallbackPolicy, void*), void*) {}

bool AbstractImporter::openData(Containers::ArrayView<const char> data) {
    CORRADE_ASSERT(features() & Feature::OpenData,
        "Trade::AbstractImporter::openData(): feature not supported", {});

    /* We accept empty data here (instead of checking for them and failing so
       the check doesn't be done on the plugin side) because for some file
       formats it could be valid (e.g. OBJ or JSON-based formats). */
    close();
    doOpenData(data);
    return isOpened();
}

void AbstractImporter::doOpenData(Containers::ArrayView<const char>) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::openData(): feature advertised but not implemented", );
}

bool AbstractImporter::openState(const void* state, const std::string& filePath) {
    CORRADE_ASSERT(features() & Feature::OpenState,
        "Trade::AbstractImporter::openState(): feature not supported", {});

    close();
    doOpenState(state, filePath);
    return isOpened();
}

void AbstractImporter::doOpenState(const void*, const std::string&) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::openState(): feature advertised but not implemented", );
}

bool AbstractImporter::openFile(const std::string& filename) {
    close();

    /* If file loading callbacks are not set or the importer supports handling
       them directly, call into the implementation */
    if(!_fileCallback || (doFeatures() & Feature::FileCallback)) {
        doOpenFile(filename);

    /* Otherwise, if loading from data is supported, use the callback and pass
       the data through to openData(). Mark the file as ready to be closed once
       opening is finished. */
    } else if(doFeatures() & Feature::OpenData) {
        /* This needs to be duplicated here and in the doOpenFile()
           implementation in order to support both following cases:
            - plugins that don't support FileCallback but have their own
              doOpenFile() implementation (callback needs to be used here,
              because the base doOpenFile() implementation might never get
              called)
            - plugins that support FileCallback but want to delegate the actual
              file loading to the default implementation (callback used in the
              base doOpenFile() implementation, because this branch is never
              taken in that case) */
        const Containers::Optional<Containers::ArrayView<const char>> data = _fileCallback(filename, InputFileCallbackPolicy::LoadTemporary, _fileCallbackUserData);
        if(!data) {
            Error() << "Trade::AbstractImporter::openFile(): cannot open file" << filename;
            return isOpened();
        }
        doOpenData(*data);
        _fileCallback(filename, InputFileCallbackPolicy::Close, _fileCallbackUserData);

    /* Shouldn't get here, the assert is fired already in setFileCallback() */
    } else CORRADE_ASSERT_UNREACHABLE(); /* LCOV_EXCL_LINE */

    return isOpened();
}

void AbstractImporter::doOpenFile(const std::string& filename) {
    CORRADE_ASSERT(features() & Feature::OpenData, "Trade::AbstractImporter::openFile(): not implemented", );

    /* If callbacks are set, use them. This is the same implementation as in
       openFile(), see the comment there for details. */
    if(_fileCallback) {
        const Containers::Optional<Containers::ArrayView<const char>> data = _fileCallback(filename, InputFileCallbackPolicy::LoadTemporary, _fileCallbackUserData);
        if(!data) {
            Error() << "Trade::AbstractImporter::openFile(): cannot open file" << filename;
            return;
        }
        doOpenData(*data);
        _fileCallback(filename, InputFileCallbackPolicy::Close, _fileCallbackUserData);

    /* Otherwise open the file directly */
    } else {
        if(!Utility::Directory::exists(filename)) {
            Error() << "Trade::AbstractImporter::openFile(): cannot open file" << filename;
            return;
        }

        doOpenData(Utility::Directory::read(filename));
    }
}

void AbstractImporter::close() {
    if(isOpened()) {
        doClose();
        CORRADE_INTERNAL_ASSERT(!isOpened());
    }
}

Int AbstractImporter::defaultScene() {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::defaultScene(): no file opened", -1);
    return doDefaultScene();
}

Int AbstractImporter::doDefaultScene() { return -1; }

UnsignedInt AbstractImporter::sceneCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::sceneCount(): no file opened", 0);
    return doSceneCount();
}

UnsignedInt AbstractImporter::doSceneCount() const { return 0; }

Int AbstractImporter::sceneForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::sceneForName(): no file opened", -1);
    return doSceneForName(name);
}

Int AbstractImporter::doSceneForName(const std::string&) { return -1; }

std::string AbstractImporter::sceneName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::sceneName(): no file opened", {});
    CORRADE_ASSERT(id < doSceneCount(), "Trade::AbstractImporter::sceneName(): index" << id << "out of range for" << doSceneCount() << "entries", {});
    return doSceneName(id);
}

std::string AbstractImporter::doSceneName(UnsignedInt) { return {}; }

Containers::Optional<SceneData> AbstractImporter::scene(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::scene(): no file opened", {});
    CORRADE_ASSERT(id < doSceneCount(), "Trade::AbstractImporter::scene(): index" << id << "out of range for" << doSceneCount() << "entries", {});
    return doScene(id);
}

Containers::Optional<SceneData> AbstractImporter::doScene(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::scene(): not implemented", {});
}

UnsignedInt AbstractImporter::animationCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::animationCount(): no file opened", {});
    return doAnimationCount();
}

UnsignedInt AbstractImporter::doAnimationCount() const { return 0; }

Int AbstractImporter::animationForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::animationForName(): no file opened", {});
    return doAnimationForName(name);
}

Int AbstractImporter::doAnimationForName(const std::string&) { return -1; }

std::string AbstractImporter::animationName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::animationName(): no file opened", {});
    CORRADE_ASSERT(id < doAnimationCount(), "Trade::AbstractImporter::animationName(): index" << id << "out of range for" << doAnimationCount() << "entries", {});
    return doAnimationName(id);
}

std::string AbstractImporter::doAnimationName(UnsignedInt) { return {}; }

Containers::Optional<AnimationData> AbstractImporter::animation(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::animation(): no file opened", {});
    CORRADE_ASSERT(id < doAnimationCount(), "Trade::AbstractImporter::animation(): index" << id << "out of range for" << doAnimationCount() << "entries", {});
    Containers::Optional<AnimationData> animation = doAnimation(id);
    CORRADE_ASSERT(!animation || (!animation->_data.deleter() && !animation->_tracks.deleter()), "Trade::AbstractImporter::animation(): implementation is not allowed to use a custom Array deleter", {});
    return animation;
}

Containers::Optional<AnimationData> AbstractImporter::doAnimation(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::animation(): not implemented", {});
}

UnsignedInt AbstractImporter::lightCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::lightCount(): no file opened", {});
    return doLightCount();
}

UnsignedInt AbstractImporter::doLightCount() const { return 0; }

Int AbstractImporter::lightForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::lightForName(): no file opened", {});
    return doLightForName(name);
}

Int AbstractImporter::doLightForName(const std::string&) { return -1; }

std::string AbstractImporter::lightName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::lightName(): no file opened", {});
    CORRADE_ASSERT(id < doLightCount(), "Trade::AbstractImporter::lightName(): index" << id << "out of range for" << doLightCount() << "entries", {});
    return doLightName(id);
}

std::string AbstractImporter::doLightName(UnsignedInt) { return {}; }

Containers::Optional<LightData> AbstractImporter::light(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::light(): no file opened", {});
    CORRADE_ASSERT(id < doLightCount(), "Trade::AbstractImporter::light(): index" << id << "out of range for" << doLightCount() << "entries", {});
    return doLight(id);
}

Containers::Optional<LightData> AbstractImporter::doLight(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::light(): not implemented", {});
}

UnsignedInt AbstractImporter::cameraCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::cameraCount(): no file opened", {});
    return doCameraCount();
}

UnsignedInt AbstractImporter::doCameraCount() const { return 0; }

Int AbstractImporter::cameraForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::cameraForName(): no file opened", {});
    return doCameraForName(name);
}

Int AbstractImporter::doCameraForName(const std::string&) { return -1; }

std::string AbstractImporter::cameraName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::cameraName(): no file opened", {});
    CORRADE_ASSERT(id < doCameraCount(), "Trade::AbstractImporter::cameraName(): index" << id << "out of range for" << doCameraCount() << "entries", {});
    return doCameraName(id);
}

std::string AbstractImporter::doCameraName(UnsignedInt) { return {}; }

Containers::Optional<CameraData> AbstractImporter::camera(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::camera(): no file opened", {});
    CORRADE_ASSERT(id < doCameraCount(), "Trade::AbstractImporter::camera(): index" << id << "out of range for" << doCameraCount() << "entries", {});
    return doCamera(id);
}

Containers::Optional<CameraData> AbstractImporter::doCamera(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::camera(): not implemented", {});
}

UnsignedInt AbstractImporter::object2DCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::object2DCount(): no file opened", {});
    return doObject2DCount();
}

UnsignedInt AbstractImporter::doObject2DCount() const { return 0; }

Int AbstractImporter::object2DForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::object2DForName(): no file opened", {});
    return doObject2DForName(name);
}

Int AbstractImporter::doObject2DForName(const std::string&) { return -1; }

std::string AbstractImporter::object2DName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::object2DName(): no file opened", {});
    CORRADE_ASSERT(id < doObject2DCount(), "Trade::AbstractImporter::object2DName(): index" << id << "out of range for" << doObject2DCount() << "entries", {});
    return doObject2DName(id);
}

std::string AbstractImporter::doObject2DName(UnsignedInt) { return {}; }

Containers::Pointer<ObjectData2D> AbstractImporter::object2D(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::object2D(): no file opened", {});
    CORRADE_ASSERT(id < doObject2DCount(), "Trade::AbstractImporter::object2D(): index" << id << "out of range for" << doObject2DCount() << "entries", {});
    return doObject2D(id);
}

Containers::Pointer<ObjectData2D> AbstractImporter::doObject2D(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::object2D(): not implemented", {});
}

UnsignedInt AbstractImporter::object3DCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::object3DCount(): no file opened", {});
    return doObject3DCount();
}

UnsignedInt AbstractImporter::doObject3DCount() const { return 0; }

Int AbstractImporter::object3DForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::object3DForName(): no file opened", {});
    return doObject3DForName(name);
}

Int AbstractImporter::doObject3DForName(const std::string&) { return -1; }

std::string AbstractImporter::object3DName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::object3DName(): no file opened", {});
    CORRADE_ASSERT(id < doObject3DCount(), "Trade::AbstractImporter::object3DName(): index" << id << "out of range for" << doObject3DCount() << "entries", {});
    return doObject3DName(id);
}

std::string AbstractImporter::doObject3DName(UnsignedInt) { return {}; }

Containers::Pointer<ObjectData3D> AbstractImporter::object3D(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::object3D(): no file opened", {});
    CORRADE_ASSERT(id < doObject3DCount(), "Trade::AbstractImporter::object3D(): index" << id << "out of range for" << doObject3DCount() << "entries", {});
    return doObject3D(id);
}

Containers::Pointer<ObjectData3D> AbstractImporter::doObject3D(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::object3D(): not implemented", {});
}

UnsignedInt AbstractImporter::mesh2DCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::mesh2DCount(): no file opened", {});
    return doMesh2DCount();
}

UnsignedInt AbstractImporter::doMesh2DCount() const { return 0; }

Int AbstractImporter::mesh2DForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::mesh2DForName(): no file opened", {});
    return doMesh2DForName(name);
}

Int AbstractImporter::doMesh2DForName(const std::string&) { return -1; }

std::string AbstractImporter::mesh2DName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::mesh2DName(): no file opened", {});
    CORRADE_ASSERT(id < doMesh2DCount(), "Trade::AbstractImporter::mesh2DName(): index" << id << "out of range for" << doMesh2DCount() << "entries", {});
    return doMesh2DName(id);
}

std::string AbstractImporter::doMesh2DName(UnsignedInt) { return {}; }

Containers::Optional<MeshData2D> AbstractImporter::mesh2D(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::mesh2D(): no file opened", {});
    CORRADE_ASSERT(id < doMesh2DCount(), "Trade::AbstractImporter::mesh2D(): index" << id << "out of range for" << doMesh2DCount() << "entries", {});
    return doMesh2D(id);
}

Containers::Optional<MeshData2D> AbstractImporter::doMesh2D(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::mesh2D(): not implemented", {});
}

UnsignedInt AbstractImporter::mesh3DCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::mesh3DCount(): no file opened", {});
    return doMesh3DCount();
}

UnsignedInt AbstractImporter::doMesh3DCount() const { return 0; }

Int AbstractImporter::mesh3DForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::mesh3DForName(): no file opened", {});
    return doMesh3DForName(name);
}

Int AbstractImporter::doMesh3DForName(const std::string&) { return -1; }

std::string AbstractImporter::mesh3DName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::mesh3DName(): no file opened", {});
    CORRADE_ASSERT(id < doMesh3DCount(), "Trade::AbstractImporter::mesh3DName(): index" << id << "out of range for" << doMesh3DCount() << "entries", {});
    return doMesh3DName(id);
}

std::string AbstractImporter::doMesh3DName(UnsignedInt) { return {}; }

Containers::Optional<MeshData3D> AbstractImporter::mesh3D(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::mesh3D(): no file opened", {});
    CORRADE_ASSERT(id < doMesh3DCount(), "Trade::AbstractImporter::mesh3D(): index" << id << "out of range for" << doMesh3DCount() << "entries", {});
    return doMesh3D(id);
}

Containers::Optional<MeshData3D> AbstractImporter::doMesh3D(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::mesh3D(): not implemented", {});
}

UnsignedInt AbstractImporter::materialCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::materialCount(): no file opened", {});
    return doMaterialCount();
}

UnsignedInt AbstractImporter::doMaterialCount() const { return 0; }

Int AbstractImporter::materialForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::materialForName(): no file opened", {});
    return doMaterialForName(name);
}

Int AbstractImporter::doMaterialForName(const std::string&) { return -1; }

std::string AbstractImporter::materialName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::materialName(): no file opened", {});
    CORRADE_ASSERT(id < doMaterialCount(), "Trade::AbstractImporter::materialName(): index" << id << "out of range for" << doMaterialCount() << "entries", {});
    return doMaterialName(id);
}

std::string AbstractImporter::doMaterialName(UnsignedInt) { return {}; }

Containers::Pointer<AbstractMaterialData> AbstractImporter::material(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::material(): no file opened", {});
    CORRADE_ASSERT(id < doMaterialCount(), "Trade::AbstractImporter::material(): index" << id << "out of range for" << doMaterialCount() << "entries", {});
    return doMaterial(id);
}

Containers::Pointer<AbstractMaterialData> AbstractImporter::doMaterial(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::material(): not implemented", {});
}

UnsignedInt AbstractImporter::textureCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::textureCount(): no file opened", {});
    return doTextureCount();
}

UnsignedInt AbstractImporter::doTextureCount() const { return 0; }

Int AbstractImporter::textureForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::textureForName(): no file opened", {});
    return doTextureForName(name);
}

Int AbstractImporter::doTextureForName(const std::string&) { return -1; }

std::string AbstractImporter::textureName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::textureName(): no file opened", {});
    CORRADE_ASSERT(id < doTextureCount(), "Trade::AbstractImporter::textureName(): index" << id << "out of range for" << doTextureCount() << "entries", {});
    return doTextureName(id);
}

std::string AbstractImporter::doTextureName(UnsignedInt) { return {}; }

Containers::Optional<TextureData> AbstractImporter::texture(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::texture(): no file opened", {});
    CORRADE_ASSERT(id < doTextureCount(), "Trade::AbstractImporter::texture(): index" << id << "out of range for" << doTextureCount() << "entries", {});
    return doTexture(id);
}

Containers::Optional<TextureData> AbstractImporter::doTexture(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::texture(): not implemented", {});
}

UnsignedInt AbstractImporter::image1DCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image1DCount(): no file opened", {});
    return doImage1DCount();
}

UnsignedInt AbstractImporter::doImage1DCount() const { return 0; }

Int AbstractImporter::image1DForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image1DForName(): no file opened", {});
    return doImage1DForName(name);
}

Int AbstractImporter::doImage1DForName(const std::string&) { return -1; }

std::string AbstractImporter::image1DName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image1DName(): no file opened", {});
    CORRADE_ASSERT(id < doImage1DCount(), "Trade::AbstractImporter::image1DName(): index" << id << "out of range for" << doImage1DCount() << "entries", {});
    return doImage1DName(id);
}

std::string AbstractImporter::doImage1DName(UnsignedInt) { return {}; }

Containers::Optional<ImageData1D> AbstractImporter::image1D(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image1D(): no file opened", {});
    CORRADE_ASSERT(id < doImage1DCount(), "Trade::AbstractImporter::image1D(): index" << id << "out of range for" << doImage1DCount() << "entries", {});
    Containers::Optional<ImageData1D> image = doImage1D(id);
    CORRADE_ASSERT(!image || !image->_data.deleter(), "Trade::AbstractImporter::image1D(): implementation is not allowed to use a custom Array deleter", {});
    return image;
}

Containers::Optional<ImageData1D> AbstractImporter::doImage1D(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::image1D(): not implemented", {});
}

UnsignedInt AbstractImporter::image2DCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image2DCount(): no file opened", {});
    return doImage2DCount();
}

UnsignedInt AbstractImporter::doImage2DCount() const { return 0; }

Int AbstractImporter::image2DForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image2DForName(): no file opened", {});
    return doImage2DForName(name);
}

Int AbstractImporter::doImage2DForName(const std::string&) { return -1; }

std::string AbstractImporter::image2DName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image2DName(): no file opened", {});
    CORRADE_ASSERT(id < doImage2DCount(), "Trade::AbstractImporter::image2DName(): index" << id << "out of range for" << doImage2DCount() << "entries", {});
    return doImage2DName(id);
}

std::string AbstractImporter::doImage2DName(UnsignedInt) { return {}; }

Containers::Optional<ImageData2D> AbstractImporter::image2D(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image2D(): no file opened", {});
    CORRADE_ASSERT(id < doImage2DCount(), "Trade::AbstractImporter::image2D(): index" << id << "out of range for" << doImage2DCount() << "entries", {});
    Containers::Optional<ImageData2D> image = doImage2D(id);
    CORRADE_ASSERT(!image || !image->_data.deleter(), "Trade::AbstractImporter::image2D(): implementation is not allowed to use a custom Array deleter", {});
    return image;
}

Containers::Optional<ImageData2D> AbstractImporter::doImage2D(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::image2D(): not implemented", {});
}

UnsignedInt AbstractImporter::image3DCount() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image3DCount(): no file opened", {});
    return doImage3DCount();
}

UnsignedInt AbstractImporter::doImage3DCount() const { return 0; }

Int AbstractImporter::image3DForName(const std::string& name) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image3DForName(): no file opened", {});
    return doImage3DForName(name);
}

Int AbstractImporter::doImage3DForName(const std::string&) { return -1; }

std::string AbstractImporter::image3DName(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image3DName(): no file opened", {});
    CORRADE_ASSERT(id < doImage3DCount(), "Trade::AbstractImporter::image3DName(): index" << id << "out of range for" << doImage3DCount() << "entries", {});
    return doImage3DName(id);
}

std::string AbstractImporter::doImage3DName(UnsignedInt) { return {}; }

Containers::Optional<ImageData3D> AbstractImporter::image3D(const UnsignedInt id) {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::image3D(): no file opened", {});
    CORRADE_ASSERT(id < doImage3DCount(), "Trade::AbstractImporter::image3D(): index" << id << "out of range for" << doImage3DCount() << "entries", {});
    Containers::Optional<ImageData3D> image = doImage3D(id);
    CORRADE_ASSERT(!image || !image->_data.deleter(), "Trade::AbstractImporter::image3D(): implementation is not allowed to use a custom Array deleter", {});
    return image;
}

Containers::Optional<ImageData3D> AbstractImporter::doImage3D(UnsignedInt) {
    CORRADE_ASSERT(false, "Trade::AbstractImporter::image3D(): not implemented", {});
}

const void* AbstractImporter::importerState() const {
    CORRADE_ASSERT(isOpened(), "Trade::AbstractImporter::importerState(): no file opened", {});
    return doImporterState();
}

const void* AbstractImporter::doImporterState() const { return nullptr; }

Debug& operator<<(Debug& debug, const AbstractImporter::Feature value) {
    debug << "Trade::AbstractImporter::Feature" << Debug::nospace;

    switch(value) {
        /* LCOV_EXCL_START */
        #define _c(v) case AbstractImporter::Feature::v: return debug << "::" #v;
        _c(OpenData)
        _c(OpenState)
        _c(FileCallback)
        #undef _c
        /* LCOV_EXCL_STOP */
    }

    return debug << "(" << Debug::nospace << reinterpret_cast<void*>(UnsignedByte(value)) << Debug::nospace << ")";
}

Debug& operator<<(Debug& debug, const AbstractImporter::Features value) {
    return Containers::enumSetDebugOutput(debug, value, "Trade::AbstractImporter::Features{}", {
        AbstractImporter::Feature::OpenData,
        AbstractImporter::Feature::OpenState,
        AbstractImporter::Feature::FileCallback});
}

}}
