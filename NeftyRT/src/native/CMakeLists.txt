

add_library(Native Generalizer.cpp native.h)

add_executable(TestNative TestNative.cpp)
add_dependencies(TestNative Native)