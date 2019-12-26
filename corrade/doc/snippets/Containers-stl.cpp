/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019 Vladimír Vondruš <mosra@centrum.cz>

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

#include "Corrade/Containers/ArrayViewStl.h"
#include "Corrade/Containers/PointerStl.h"
#include "Corrade/Containers/ReferenceStl.h"

using namespace Corrade;

int main() {
{
/* [ArrayView] */
std::vector<int> a;

Containers::ArrayView<int> b = a;
/* [ArrayView] */
static_cast<void>(b);
}

{
/* [Pointer] */
std::unique_ptr<int> a{new int{5}};
Containers::Pointer<int> b = std::move(a);

std::unique_ptr<int> c = Containers::pointer<int>(12);

auto d = Containers::pointer(std::unique_ptr<int>{new int{5}});
        // d is Containers::Pointer<int>
/* [Pointer] */
}

{
/* [Reference] */
int a = 1337;
Containers::Reference<int> b = a;

std::reference_wrapper<int> c = b;
Containers::Reference<const int> d = std::cref(a);
/* [Reference] */
static_cast<void>(c);
static_cast<void>(d);
}
}
