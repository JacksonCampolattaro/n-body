
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



# Include our targets
add_subdirectory(src)
#add_subdirectory(docs)
