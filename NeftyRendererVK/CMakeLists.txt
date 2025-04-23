add_library(NeftyRendererVK STATIC NeftyRendererVK.h Context.cpp)

target_include_directories(NeftyRendererVK PUBLIC ${VULKAN_INCLUDE} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_directories(NeftyRendererVK PUBLIC ${VULKAN_LIB_DIR})
target_link_libraries(NeftyRendererVK PUBLIC ${VULKAN_LIBRARIES})
