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

#include <sstream>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/DebugStl.h>

#include "Magnum/SceneGraph/DualQuaternionTransformation.h"
#include "Magnum/SceneGraph/Scene.h"

namespace Magnum { namespace SceneGraph { namespace Test { namespace {

typedef Object<DualQuaternionTransformation> Object3D;
typedef Scene<DualQuaternionTransformation> Scene3D;

struct DualQuaternionTransformationTest: TestSuite::Tester {
    explicit DualQuaternionTransformationTest();

    void fromMatrix();
    void toMatrix();
    void compose();
    void inverted();

    void setTransformation();
    void resetTransformation();
    void transform();
    void translate();
    void rotate();
    void normalizeRotation();
};

DualQuaternionTransformationTest::DualQuaternionTransformationTest() {
    addTests({&DualQuaternionTransformationTest::fromMatrix,
              &DualQuaternionTransformationTest::toMatrix,
              &DualQuaternionTransformationTest::compose,
              &DualQuaternionTransformationTest::inverted,

              &DualQuaternionTransformationTest::setTransformation,
              &DualQuaternionTransformationTest::resetTransformation,
              &DualQuaternionTransformationTest::transform,
              &DualQuaternionTransformationTest::translate,
              &DualQuaternionTransformationTest::rotate,
              &DualQuaternionTransformationTest::normalizeRotation});
}

void DualQuaternionTransformationTest::fromMatrix() {
    std::ostringstream o;
    Error redirectError{&o};
    Implementation::Transformation<DualQuaternionTransformation>::fromMatrix(Matrix4::scaling(Vector3(4.0f)));
    CORRADE_COMPARE(o.str(), "SceneGraph::DualQuaternionTransformation: the matrix doesn't represent rigid transformation\n");

    Matrix4 m = Matrix4::rotationX(Deg(17.0f))*Matrix4::translation({1.0f, -0.3f, 2.3f});
    DualQuaternion q = DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis())*DualQuaternion::translation({1.0f, -0.3f, 2.3f});
    CORRADE_COMPARE(Implementation::Transformation<DualQuaternionTransformation>::fromMatrix(m), q);
}

void DualQuaternionTransformationTest::toMatrix() {
    DualQuaternion q = DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis())*DualQuaternion::translation({1.0f, -0.3f, 2.3f});
    Matrix4 m = Matrix4::rotationX(Deg(17.0f))*Matrix4::translation({1.0f, -0.3f, 2.3f});
    CORRADE_COMPARE(Implementation::Transformation<DualQuaternionTransformation>::toMatrix(q), m);
}

void DualQuaternionTransformationTest::compose() {
    DualQuaternion parent = DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis());
    DualQuaternion child = DualQuaternion::translation({1.0f, -0.3f, 2.3f});
    CORRADE_COMPARE(Implementation::Transformation<DualQuaternionTransformation>::compose(parent, child), parent*child);
}

void DualQuaternionTransformationTest::inverted() {
    DualQuaternion q = DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis())*DualQuaternion::translation({1.0f, -0.3f, 2.3f});
    CORRADE_COMPARE(Implementation::Transformation<DualQuaternionTransformation>::inverted(q)*q, DualQuaternion());
}

void DualQuaternionTransformationTest::setTransformation() {
    Object3D o;

    /* Can't transform with non-rigid transformation */
    std::ostringstream out;
    Error redirectError{&out};
    o.setTransformation(DualQuaternion({{1.0f, 2.0f, 3.0f}, 4.0f}, {}));
    CORRADE_COMPARE(out.str(), "SceneGraph::DualQuaternionTransformation::setTransformation(): the dual quaternion is not normalized\n");

    /* Dirty after setting transformation */
    o.setClean();
    CORRADE_VERIFY(!o.isDirty());
    o.setTransformation(DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis()));
    CORRADE_VERIFY(o.isDirty());
    CORRADE_COMPARE(o.transformationMatrix(), Matrix4::rotationX(Deg(17.0f)));

    /* Scene cannot be transformed */
    Scene3D s;
    s.setClean();
    CORRADE_VERIFY(!s.isDirty());
    s.setTransformation(DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis()));
    CORRADE_VERIFY(!s.isDirty());
    CORRADE_COMPARE(s.transformationMatrix(), Matrix4());
}

void DualQuaternionTransformationTest::resetTransformation() {
    Object3D o;
    o.setTransformation(DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis()));
    CORRADE_VERIFY(o.transformationMatrix() != Matrix4());
    o.resetTransformation();
    CORRADE_COMPARE(o.transformationMatrix(), Matrix4());
}

void DualQuaternionTransformationTest::transform() {
    {
        /* Can't transform with non-rigid transformation */
        Object3D o;
        std::ostringstream out;
        Error redirectError{&out};
        o.transform(DualQuaternion({{1.0f, 2.0f, 3.0f}, 4.0f}, {}));
        CORRADE_COMPARE(out.str(), "SceneGraph::DualQuaternionTransformation::transform(): the dual quaternion is not normalized\n");
    } {
        Object3D o;
        o.setTransformation(DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis()));
        o.transform(DualQuaternion::translation({1.0f, -0.3f, 2.3f}));
        CORRADE_COMPARE(o.transformationMatrix(), Matrix4::translation({1.0f, -0.3f, 2.3f})*Matrix4::rotationX(Deg(17.0f)));
    } {
        Object3D o;
        o.setTransformation(DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis()));
        o.transformLocal(DualQuaternion::translation({1.0f, -0.3f, 2.3f}));
        CORRADE_COMPARE(o.transformationMatrix(), Matrix4::rotationX(Deg(17.0f))*Matrix4::translation({1.0f, -0.3f, 2.3f}));
    }
}

void DualQuaternionTransformationTest::translate() {
    {
        Object3D o;
        o.setTransformation(DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis()));
        o.translate({1.0f, -0.3f, 2.3f});
        CORRADE_COMPARE(o.transformationMatrix(), Matrix4::translation({1.0f, -0.3f, 2.3f})*Matrix4::rotationX(Deg(17.0f)));
    } {
        Object3D o;
        o.setTransformation(DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis()));
        o.translateLocal({1.0f, -0.3f, 2.3f});
        CORRADE_COMPARE(o.transformationMatrix(), Matrix4::rotationX(Deg(17.0f))*Matrix4::translation({1.0f, -0.3f, 2.3f}));
    }
}

void DualQuaternionTransformationTest::rotate() {
    {
        Object3D o;
        o.transform(DualQuaternion::translation({1.0f, -0.3f, 2.3f}));
        o.rotateX(Deg(17.0f))
            .rotateY(Deg(25.0f))
            .rotateZ(Deg(-23.0f))
            .rotate(Deg(96.0f), Vector3(1.0f/Constants::sqrt3()));
        CORRADE_COMPARE(o.transformationMatrix(),
            Matrix4::rotation(Deg(96.0f), Vector3(1.0f/Constants::sqrt3()))*
            Matrix4::rotationZ(Deg(-23.0f))*
            Matrix4::rotationY(Deg(25.0f))*
            Matrix4::rotationX(Deg(17.0f))*
            Matrix4::translation({1.0f, -0.3f, 2.3f}));
    } {
        Object3D o;
        o.transform(DualQuaternion::translation({1.0f, -0.3f, 2.3f}));
        o.rotateXLocal(Deg(17.0f))
            .rotateYLocal(Deg(25.0f))
            .rotateZLocal(Deg(-23.0f))
            .rotateLocal(Deg(96.0f), Vector3(1.0f/Constants::sqrt3()));
        CORRADE_COMPARE(o.transformationMatrix(),
            Matrix4::translation({1.0f, -0.3f, 2.3f})*
            Matrix4::rotationX(Deg(17.0f))*
            Matrix4::rotationY(Deg(25.0f))*
            Matrix4::rotationZ(Deg(-23.0f))*
            Matrix4::rotation(Deg(96.0f), Vector3(1.0f/Constants::sqrt3())));
    }
}

void DualQuaternionTransformationTest::normalizeRotation() {
    Object3D o;
    o.setTransformation(DualQuaternion::rotation(Deg(17.0f), Vector3::xAxis()));
    o.normalizeRotation();
    CORRADE_COMPARE(o.transformationMatrix(), Matrix4::rotationX(Deg(17.0f)));
}

}}}}

CORRADE_TEST_MAIN(Magnum::SceneGraph::Test::DualQuaternionTransformationTest)
