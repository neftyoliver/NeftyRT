


add_library(NeftyRT src/Context.cpp)

if (${WIN32})
    target_include_directories(NeftyRT PUBLIC include ../glfw/include $ENV{VK_SDK_PATH}/Include)
endif ()

if (${APPLE})
    target_include_directories(NeftyRT PUBLIC include ../glfw/include "/Users/$ENV{USER}/VulkanSDK/1.4.309.0/macOS/include")
    target_link_directories(NeftyRT PUBLIC $ENV{USER}/VulkanSDK/1.4.309.0/macOS/lib)
    target_link_libraries(NeftyRT PUBLIC vulkan SDL2)
endif ()