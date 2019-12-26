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

#include "Magnum/Shaders/VertexColor.h"

namespace Magnum { namespace Shaders { namespace Test { namespace {

struct VertexColorTest: TestSuite::Tester {
    explicit VertexColorTest();

    void constructNoCreate2D();
    void constructNoCreate3D();

    void constructCopy2D();
    void constructCopy3D();
};

VertexColorTest::VertexColorTest() {
    addTests({&VertexColorTest::constructNoCreate2D,
              &VertexColorTest::constructNoCreate3D,

              &VertexColorTest::constructCopy2D,
              &VertexColorTest::constructCopy3D});
}

void VertexColorTest::constructNoCreate2D() {
    {
        VertexColor2D shader{NoCreate};
        CORRADE_COMPARE(shader.id(), 0);
    }

    CORRADE_VERIFY(true);
}

void VertexColorTest::constructNoCreate3D() {
    {
        VertexColor3D shader{NoCreate};
        CORRADE_COMPARE(shader.id(), 0);
    }

    CORRADE_VERIFY(true);
}

void VertexColorTest::constructCopy2D() {
    CORRADE_VERIFY(!(std::is_constructible<VertexColor2D, const VertexColor2D&>{}));
    CORRADE_VERIFY(!(std::is_assignable<VertexColor2D, const VertexColor2D&>{}));
}

void VertexColorTest::constructCopy3D() {
    CORRADE_VERIFY(!(std::is_constructible<VertexColor3D, const VertexColor3D&>{}));
    CORRADE_VERIFY(!(std::is_assignable<VertexColor3D, const VertexColor3D&>{}));
}

}}}}

CORRADE_TEST_MAIN(Magnum::Shaders::Test::VertexColorTest)
