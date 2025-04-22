cmake_minimum_required(VERSION 3.30)

project(NeftyRT C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)


if(${WIN32})
    set(VULKAN_SDK $ENV{VK_SDK_PATH})
endif ()

add_subdirectory(glslang)
add_subdirectory(tinyobjloader)
add_subdirectory(glfw)

add_library(imgui STATIC
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp

        imgui/misc/cpp/imgui_stdlib.cpp
        imgui/misc/cpp/imgui_stdlib.h

        imgui/misc/single_file/imgui_single_file.h

        imgui/backends/imgui_impl_vulkan.cpp
        imgui/backends/imgui_impl_vulkan.h
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_glfw.h
)
target_include_directories(imgui
        PUBLIC imgui
        PUBLIC glfw/include
        PUBLIC ${VULKAN_SDK}/Include
)




#[==[
The main project of this repo.
]==]

add_subdirectory(NeftyRendererVK)

add_executable(NeftyRT
        src/main.cpp
)

add_dependencies(NeftyRT NeftyRendererVK glfw glslang)


