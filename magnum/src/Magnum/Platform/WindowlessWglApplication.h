#ifndef Magnum_Platform_WindowlessWglApplication_h
#define Magnum_Platform_WindowlessWglApplication_h
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

/** @file
 * @brief Class @ref Magnum::Platform::WindowlessWglApplication, @ref Magnum::Platform::WindowlessWglContext, macro @ref MAGNUM_WINDOWLESSWGLAPPLICATION_MAIN()
 */

#include "Magnum/configure.h"

#ifdef MAGNUM_TARGET_GL
#ifndef DOXYGEN_GENERATING_OUTPUT
#define WIN32_LEAN_AND_MEAN 1
#define VC_EXTRALEAN
#endif
#include <windows.h>
#include <Corrade/Containers/EnumSet.h>
#include <Corrade/Containers/Pointer.h>

#include "Magnum/Magnum.h"
#include "Magnum/Tags.h"
#include "Magnum/GL/OpenGL.h"
#include "Magnum/Platform/Platform.h"

#ifndef DOXYGEN_GENERATING_OUTPUT
/* Define stuff that we need because I can't be bothered with creating a new
   header just for a few defines */
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#endif

namespace Magnum { namespace Platform {

/**
@brief Windowless WGL context

@m_keywords{WindowlessGLContext}

GL context using pure WINAPI, used in @ref WindowlessWglApplication. It is
built if `WITH_WINDOWLESSWGLAPPLICATION` is enabled in CMake.

Meant to be used when there is a need to manage (multiple) GL contexts
manually. See @ref platform-windowless-contexts for more information. If no
other application header is included, this class is also aliased to
@cpp Platform::WindowlessGLContext @ce.

@note This class is available only if Magnum is compiled with
    @ref MAGNUM_TARGET_GL enabled (done by default). See @ref building-features
    for more information.
*/
class WindowlessWglContext {
    public:
        class Configuration;

        /**
         * @brief Constructor
         * @param configuration Context configuration
         * @param context       Optional Magnum context instance constructed
         *      using @ref NoCreate to manage driver workarounds
         *
         * On desktop GL, if version is not specified in @p configuration, the
         * application first tries to create core context (OpenGL 3.1+) and if
         * that fails, falls back to compatibility OpenGL 2.1 context. However,
         * on binary AMD and NVidia drivers, creating core context does not use
         * the largest available version. If the application detects such case
         * (and given workaround is not disabled in optionally passed
         * @p context instance), the core context is destroyed and
         * compatibility OpenGL 2.1 context is created instead to make the
         * driver use the latest available version.
         *
         * Once the context is created, make it current using
         * @ref makeCurrent() and create @ref Platform::GLContext instance to
         * be able to use Magnum.
         * @see @ref isCreated()
         */
        explicit WindowlessWglContext(const Configuration& configuration, GLContext* context = nullptr);

        /**
         * @brief Construct without creating the context
         *
         * Move a instance with created context over to make it usable.
         */
        explicit WindowlessWglContext(NoCreateT) {}

        /** @brief Copying is not allowed */
        WindowlessWglContext(const WindowlessWglContext&) = delete;

        /** @brief Move constructor */
        WindowlessWglContext(WindowlessWglContext&& other);

        /** @brief Copying is not allowed */
        WindowlessWglContext& operator=(const WindowlessWglContext&) = delete;

        /** @brief Move assignment */
        WindowlessWglContext& operator=(WindowlessWglContext&& other);

        /**
         * @brief Destructor
         *
         * Destroys the context, if any.
         */
        ~WindowlessWglContext();

        /** @brief Whether the context is created */
        bool isCreated() const { return _context; }

        /**
         * @brief Make the context current
         *
         * Prints error message and returns @cpp false @ce on failure,
         * otherwise returns @cpp true @ce.
         */
        bool makeCurrent();

    private:
        HWND _window{};
        HDC _deviceContext{};
        HGLRC _context{};
};

/**
@brief Configuration

@see @ref WindowlessWglContext(),
    @ref WindowlessWglApplication::WindowlessWglApplication(),
    @ref WindowlessWglApplication::createContext(),
    @ref WindowlessWglApplication::tryCreateContext()
*/
class WindowlessWglContext::Configuration {
    public:
        /**
         * @brief Context flag
         *
         * @see @ref Flags, @ref setFlags(), @ref GL::Context::Flag
         */
        enum class Flag: int {
            #ifndef MAGNUM_TARGET_GLES
            /**
             * Forward compatible context
             *
             * @requires_gl Core/compatibility profile distinction and forward
             *      compatibility applies only to desktop GL.
             */
            ForwardCompatible = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            #endif

            /**
             * Debug context. Enabled automatically if the
             * `--magnum-gpu-validation` @ref GL-Context-command-line "command-line option"
             * is present.
             */
            Debug = WGL_CONTEXT_DEBUG_BIT_ARB
        };

        /**
         * @brief Context flags
         *
         * @see @ref setFlags(), @ref Context::Flags
         */
        #ifndef DOXYGEN_GENERATING_OUTPUT
        typedef Containers::EnumSet<Flag, WGL_CONTEXT_DEBUG_BIT_ARB
            #ifndef MAGNUM_TARGET_GLES
            |WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
            #endif
            > Flags;
        #else
        typedef Containers::EnumSet<Flag> Flags;
        #endif

        /*implicit*/ Configuration();

        /** @brief Context flags */
        Flags flags() const { return _flags; }

        /**
         * @brief Set context flags
         * @return Reference to self (for method chaining)
         *
         * Default is @ref Flag::ForwardCompatible on desktop GL and no flags
         * on OpenGL ES.
         * @see @ref addFlags(), @ref clearFlags(), @ref GL::Context::flags()
         */
        Configuration& setFlags(Flags flags) {
            _flags = flags;
            return *this;
        }

        /**
         * @brief Add context flags
         * @return Reference to self (for method chaining)
         *
         * Unlike @ref setFlags(), ORs the flags with existing instead of
         * replacing them. Useful for preserving the defaults.
         * @see @ref clearFlags()
         */
        Configuration& addFlags(Flags flags) {
            _flags |= flags;
            return *this;
        }

        /**
         * @brief Clear context flags
         * @return Reference to self (for method chaining)
         *
         * Unlike @ref setFlags(), ANDs the inverse of @p flags with existing
         * instead of replacing them. Useful for removing default flags.
         * @see @ref addFlags()
         */
        Configuration& clearFlags(Flags flags) {
            _flags &= ~flags;
            return *this;
        }

    private:
        Flags _flags;
};

CORRADE_ENUMSET_OPERATORS(WindowlessWglContext::Configuration::Flags)

/**
@brief Windowless WGL application

@m_keywords{WindowlessApplication}

Application for offscreen rendering using @ref WindowlessWglContext. This
application library is available on desktop OpenGL on Windows. It is built if
`WITH_WINDOWLESSWGLAPPLICATION` is enabled in CMake.

@section Platform-WindowlessWglApplication-bootstrap Bootstrap application

Fully contained windowless application using @ref WindowlessWglApplication
along with CMake setup is available in `windowless` branch of
[Magnum Bootstrap](https://github.com/mosra/magnum-bootstrap) repository,
download it as [tar.gz](https://github.com/mosra/magnum-bootstrap/archive/windowless.tar.gz)
or [zip](https://github.com/mosra/magnum-bootstrap/archive/windowless.zip)
file. After extracting the downloaded archive you can build and run the
application with these four commands:

@code{.bat}
mkdir build && cd build
cmake ..
cmake --build .
./src/MyApplication # or ./src/Debug/MyApplication
@endcode

See @ref cmake for more information.

@section Platform-WindowlessWglApplication-usage General usage

In order to use this library from CMake, you need to request the
`WindowlessWglApplication` component of the `Magnum` package and link to the `Magnum::WindowlessWglApplication` target:

@code{.cmake}
find_package(Magnum REQUIRED)
if(CORRADE_TARGET_WINDOWS)
    find_package(Magnum REQUIRED WindowlessWglApplication)
endif()

# ...
if(CORRADE_TARGET_WINDOWS)
    target_link_libraries(your-app Magnum::WindowlessWglApplication)
endif()
@endcode

If no other application is requested, you can also use the generic
`Magnum::WindowlessApplication` alias to simplify porting. Again, see
@ref building and @ref cmake for more information.

Place your code into @ref exec(). The subclass can be then used in @cpp main() @ce
function using @ref MAGNUM_WINDOWLESSWGLAPPLICATION_MAIN() macro. See
@ref platform for more information.

@code{.cpp}
class MyApplication: public Platform::WindowlessWglApplication {
    // implement required methods...
};
MAGNUM_WINDOWLESSWGLAPPLICATION_MAIN(MyApplication)
@endcode

If no other application header is included, this class is also aliased to
@cpp Platform::WindowlessApplication @ce and the macro is aliased to
@cpp MAGNUM_WINDOWLESSAPPLICATION_MAIN() @ce to simplify porting.

@note This class is available only if Magnum is compiled with
    @ref MAGNUM_TARGET_GL enabled (done by default). See @ref building-features
    for more information.
*/
class WindowlessWglApplication {
    public:
        /** @brief Application arguments */
        struct Arguments {
            /** @brief Constructor */
            /*implicit*/ constexpr Arguments(int& argc, char** argv) noexcept: argc{argc}, argv{argv} {}

            int& argc;      /**< @brief Argument count */
            char** argv;    /**< @brief Argument values */
        };

        /**
         * @brief Configuration
         *
         * @see @ref WindowlessWglApplication(), @ref createContext(),
         *      @ref tryCreateContext()
         */
        typedef WindowlessWglContext::Configuration Configuration;

        /**
         * @brief Default constructor
         * @param arguments     Application arguments
         * @param configuration Configuration
         *
         * Creates application with default or user-specified configuration.
         * See @ref Configuration for more information. The program exits if
         * the context cannot be created, see @ref tryCreateContext() for an
         * alternative.
         * @see @ref WindowlessWglContext
         */
        #ifdef DOXYGEN_GENERATING_OUTPUT
        explicit WindowlessWglApplication(const Arguments& arguments, const Configuration& configuration = Configuration());
        #else
        /* To avoid "invalid use of incomplete type" */
        explicit WindowlessWglApplication(const Arguments& arguments, const Configuration& configuration);
        explicit WindowlessWglApplication(const Arguments& arguments);
        #endif

        /**
         * @brief Constructor
         * @param arguments     Application arguments
         *
         * Unlike above, the context is not created and must be created later
         * with @ref createContext() or @ref tryCreateContext().
         */
        explicit WindowlessWglApplication(const Arguments& arguments, NoCreateT);

        /** @brief Copying is not allowed */
        WindowlessWglApplication(const WindowlessWglApplication&) = delete;

        /** @brief Moving is not allowed */
        WindowlessWglApplication(WindowlessWglApplication&&) = delete;

        /** @brief Copying is not allowed */
        WindowlessWglApplication& operator=(const WindowlessWglApplication&) = delete;

        /** @brief Moving is not allowed */
        WindowlessWglApplication& operator=(WindowlessWglApplication&&) = delete;

        /**
         * @brief Execute application
         * @return Value for returning from `main()`
         *
         * See @ref MAGNUM_WINDOWLESSWGLAPPLICATION_MAIN() for usage
         * information.
         */
        virtual int exec() = 0;

    protected:
        /* Nobody will need to have (and delete) WindowlessWglApplication*,
           thus this is faster than public pure virtual destructor */
        ~WindowlessWglApplication();

        /**
         * @brief Create context with given configuration
         *
         * Must be called if and only if the context wasn't created by the
         * constructor itself. Error message is printed and the program exits
         * if the context cannot be created, see @ref tryCreateContext() for an
         * alternative.
         * @see @ref WindowlessWglContext
         */
        #ifdef DOXYGEN_GENERATING_OUTPUT
        void createContext(const Configuration& configuration = Configuration());
        #else
        /* To avoid "invalid use of incomplete type" */
        void createContext(const Configuration& configuration);
        void createContext();
        #endif

        /**
         * @brief Try to create context with given configuration
         *
         * Unlike @ref createContext() returns @cpp false @ce if the context
         * cannot be created, @cpp true @ce otherwise.
         */
        bool tryCreateContext(const Configuration& configuration);

    private:
        WindowlessWglContext _glContext;
        Containers::Pointer<Platform::GLContext> _context;
};

/** @hideinitializer
@brief Entry point for windowless WGL application
@param className Class name

See @ref Magnum::Platform::WindowlessWglApplication "Platform::WindowlessWglApplication"
for usage information. This macro abstracts out platform-specific entry point
code and is equivalent to the following, see @ref portability-applications for
more information.

@code{.cpp}
int main(int argc, char** argv) {
    className app({argc, argv});
    return app.exec();
}
@endcode

When no other windowless application header is included this macro is also
aliased to @cpp MAGNUM_WINDOWLESSAPPLICATION_MAIN() @ce.
*/
#define MAGNUM_WINDOWLESSWGLAPPLICATION_MAIN(className)                     \
    int main(int argc, char** argv) {                                       \
        className app({argc, argv});                                        \
        return app.exec();                                                  \
    }

#ifndef DOXYGEN_GENERATING_OUTPUT
#ifndef MAGNUM_WINDOWLESSAPPLICATION_MAIN
typedef WindowlessWglApplication WindowlessApplication;
typedef WindowlessWglContext WindowlessGLContext;
#define MAGNUM_WINDOWLESSAPPLICATION_MAIN(className) MAGNUM_WINDOWLESSWGLAPPLICATION_MAIN(className)
#else
#undef MAGNUM_WINDOWLESSAPPLICATION_MAIN
#endif
#endif

}}
#else
#error this header is available only in the OpenGL build
#endif

#endif
