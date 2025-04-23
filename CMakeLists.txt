cmake_minimum_required(VERSION 3.30)

project(NeftyRT C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)


if(${WIN32})
    set(VULKAN_BIN_DIR "$ENV{VK_SDK_PATH}/Bin")
    set(VULKAN_INCLUDE "$ENV{VK_SDK_PATH}/Include")
    set(VULKAN_LIB_DIR "$ENV{VK_SDK_PATH}/Lib")
    set(VULKAN_LIBRARIES vulkan-1)
endif ()

#add_subdirectory(glslang)
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
        PUBLIC ${VULKAN_INCLUDE}
)

add_subdirectory(NeftyRendererVK)

function(add_program target)
    add_dependencies(${target} NeftyRendererVK glfw)

    target_include_directories(${target}
            PRIVATE include
            PRIVATE NeftyRendererVK
            PRIVATE glfw/include
            PRIVATE ${VULKAN_INCLUDE}
    )
    target_link_directories(${target} PRIVATE ${VULKAN_LIB_DIR})
    target_link_libraries(${target} PRIVATE NeftyRendererVK PRIVATE glfw PUBLIC ${VULKAN_LIBRARIES})
endfunction()

function(registerShader theTarget parShaderFileName)
    find_file(shaderFileName NAMES ${parShaderFileName} PATHS "${CMAKE_SOURCE_DIR}/shaders")
    set(FileTypedName ${parShaderFileName})



    if (NOT ${shaderFileName} STREQUAL "shaderFileName-NOTFOUND")
        message(STATUS "Shader source code ${shaderFileName} exist!")

        if(NOT EXISTS shader_cash/${shaderFileName})
            message("creating shader cash in ${shaderFileName}")
            if (NOT EXISTS shader_cash)
                file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/shader_cash)
            endif ()
            file(COPY ${shaderFileName} DESTINATION "${CMAKE_SOURCE_DIR}/shader_cash")
        endif ()

        if (shaders/${FileTypedName} IS_NEWER_THAN shader_cash/${FileTypedName})
            file(COPY ${shaderFileName} DESTINATION "${CMAKE_SOURCE_DIR}/shader_cash")
            set(PURE_NAME "")
            get_filename_component(PURE_NAME ${FileTypedName} NAME_WE)
            add_custom_command(
                    OUTPUT "${PURE_NAME}.spv"
                    COMMAND "${VULKAN_BIN_DIR}/glslc" ${shaderFileName} --target-env="vulkan1.4" -o "${PURE_NAME}.spv" -w
                    DEPENDS ${shaderFileName}
                    COMMENT "Compiling new ${FileTypedName} file."
            )
            add_custom_target("${PURE_NAME}_TARGET" DEPENDS "${PURE_NAME}.spv")
            add_dependencies(${theTarget} "${PURE_NAME}_TARGET")

        endif ()
    else()
        #Not a shader type
        message(WARNING "${shaderFileName} is not a shader type file.")
    endif ()

endfunction()

#[==[
The main project of this repo.
]==]




add_executable(GraphicsBasic src/GraphicsBasic.cpp)
add_program(GraphicsBasic)
registerShader(GraphicsBasic GraphicsBasicFragment.frag)
registerShader(GraphicsBasic GraphicsBasicVertex.vert)