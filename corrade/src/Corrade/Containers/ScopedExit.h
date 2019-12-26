#ifndef Corrade_Containers_ScopedExit_h
#define Corrade_Containers_ScopedExit_h
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
 * @m_deprecated_since{2019,01} Use @ref Corrade/Containers/ScopeGuard.h instead.
 */

#include "Corrade/configure.h"

#ifdef CORRADE_BUILD_DEPRECATED
#include "Corrade/Containers/ScopeGuard.h"
#include "Corrade/Utility/Macros.h"
CORRADE_DEPRECATED_FILE("use Corrade/Containers/ScopeGuard.h instead")

namespace Corrade { namespace Containers {

/** @brief @copybrief ScopeGuard
 * @m_deprecated_since{2019,01} Use @ref ScopeGuard instead.
 */
typedef CORRADE_DEPRECATED("use ScopeGuard instead") ScopeGuard ScopedExit;

}}
#else
#error use Corrade/Containers/ScopeGuard.h instead
#endif

#endif
