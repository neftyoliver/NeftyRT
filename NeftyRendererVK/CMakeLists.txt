add_library(NeftyRendererVK STATIC NeftyRendererVK.h Context.cpp)

target_include_directories(NeftyRendererVK PRIVATE ${VULKAN_INCLUDE} PRIVATE ${CMAKE_SOURCE_DIR}/include)