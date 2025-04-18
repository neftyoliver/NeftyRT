add_subdirectory(src/native)


add_library(NeftyRT src/Context.cpp)
target_include_directories(NeftyRT PUBLIC include ../glfw/include $ENV{VK_SDK_PATH}/Include)
