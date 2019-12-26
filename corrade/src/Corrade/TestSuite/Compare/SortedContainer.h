#ifndef Corrade_TestSuite_Compare_SortedContainer_h
#define Corrade_TestSuite_Compare_SortedContainer_h
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

/** @file
 * @brief Class @ref Corrade::TestSuite::Compare::SortedContainer
 * @m_since{2019,10}
 */

#include <algorithm>

#include "Corrade/TestSuite/Compare/Container.h"

namespace Corrade { namespace TestSuite {

namespace Compare {

/**
@brief Pseudo-type for comparing sorted container contents

See @ref Container for more information.
*/
template<class T> class SortedContainer: public Container<T> {};

}

#ifndef DOXYGEN_GENERATING_OUTPUT
template<class T> class Comparator<Compare::SortedContainer<T>>: public Comparator<Compare::Container<T>> {
    public:
        ComparisonStatusFlags operator()(const T& actual, const T& expected);

    private:
        T _actualSorted, _expectedSorted;
};

template<class T> ComparisonStatusFlags Comparator<Compare::SortedContainer<T>>::operator()(const T& actual, const T& expected) {
    _actualSorted = actual;
    _expectedSorted = expected;

    std::sort(_actualSorted.begin(), _actualSorted.end());
    std::sort(_expectedSorted.begin(), _expectedSorted.end());

    return Comparator<Compare::Container<T>>::operator()(_actualSorted, _expectedSorted);
}
#endif

}}

#endif

