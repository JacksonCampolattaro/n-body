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

#include <Corrade/TestSuite/Tester.h>

#include "Magnum/Shaders/DistanceFieldVector.h"

namespace Magnum { namespace Shaders { namespace Test { namespace {

struct DistanceFieldVectorTest: TestSuite::Tester {
    explicit DistanceFieldVectorTest();

    void constructNoCreate2D();
    void constructNoCreate3D();

    void constructCopy2D();
    void constructCopy3D();
};

DistanceFieldVectorTest::DistanceFieldVectorTest() {
    addTests({&DistanceFieldVectorTest::constructNoCreate2D,
              &DistanceFieldVectorTest::constructNoCreate3D,

              &DistanceFieldVectorTest::constructCopy2D,
              &DistanceFieldVectorTest::constructCopy3D});
}

void DistanceFieldVectorTest::constructNoCreate2D() {
    {
        DistanceFieldVector2D shader{NoCreate};
        CORRADE_COMPARE(shader.id(), 0);
    }

    CORRADE_VERIFY(true);
}

void DistanceFieldVectorTest::constructNoCreate3D() {
    {
        DistanceFieldVector3D shader{NoCreate};
        CORRADE_COMPARE(shader.id(), 0);
    }

    CORRADE_VERIFY(true);
}

void DistanceFieldVectorTest::constructCopy2D() {
    CORRADE_VERIFY(!(std::is_constructible<DistanceFieldVector2D, const DistanceFieldVector2D&>{}));
    CORRADE_VERIFY(!(std::is_assignable<DistanceFieldVector2D, const DistanceFieldVector2D&>{}));
}

void DistanceFieldVectorTest::constructCopy3D() {
    CORRADE_VERIFY(!(std::is_constructible<DistanceFieldVector3D, const DistanceFieldVector3D&>{}));
    CORRADE_VERIFY(!(std::is_assignable<DistanceFieldVector3D, const DistanceFieldVector3D&>{}));
}

}}}}

CORRADE_TEST_MAIN(Magnum::Shaders::Test::DistanceFieldVectorTest)
