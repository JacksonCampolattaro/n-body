
# Set minimum CMake version
cmake_minimum_required(VERSION 3.14)

# Set project name
project(n_body)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)

# todo: somehow this is actually slower? -fassociative-math would be nice, but it's not in LLVM
#set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

# todo: this is slower, too?
#set(CMAKE_CXX_FLAGS_RELEASE "-march=native+simd")

#set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra")

# Make sure catch is present
find_package(Catch2 3.1.0)
if(NOT Catch2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_SHALLOW    TRUE
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.4.0
            OVERRIDE_FIND_PACKAGE
    )
    FetchContent_MakeAvailable(Catch2)
endif()

# Include our targets
add_subdirectory(src)
#add_subdirectory(docs)
