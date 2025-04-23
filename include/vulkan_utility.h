
#pragma once

/***
 *
 * WTFPL Public license
 *
 * 0. You just DO WHAT THE **** YOU WANT TO.
 *
 */

/**
 *
 */

#include <fstream>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

#define PRINT_INFO

namespace vkut {

    inline vk::Bool32 checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, NULL);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());



        for (uint32_t i = 0; i < layerCount; i++) {
            if (std::strcmp(availableLayers[i].layerName, "VK_LAYER_KHRONOS_validation") != 0) {
                std::cerr << "Validation layer supports!" << availableLayers.at(i).layerName << std::endl;
                return VK_TRUE;
            }
        }
        return VK_FALSE;
    }

    inline void showInstanceLayers() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, NULL);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (uint32_t i = 0; i < layerCount; i++) {
            std::cout << availableLayers.at(i).layerName << std::endl;
        }
    }

    inline void showInstanceExtensions() {
        uint32_t propertyCount;
        std::vector<VkExtensionProperties> pProperties;

        vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
        pProperties.resize(propertyCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, pProperties.data());

        for (uint32_t i = 0; i < propertyCount; i += 1) {
            std::cout << pProperties.at(i).extensionName << std::endl;
        }
    }

    inline uint32_t autoSelectQueueFamily(VkPhysicalDevice physicalDevice, uint32_t deviceNumber) {
        uint32_t queueFamilySelection = 0;

        uint32_t queueFamilyPropertiesCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamiliyProperties(queueFamilyPropertiesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, queueFamiliyProperties.data());

        for (uint32_t i = 0; i < queueFamilyPropertiesCount; i += 1) {
            #ifdef PRINT_INFO
            std::cout << "QueueFamily on " << i  << " supports " << queueFamiliyProperties.at(i).queueCount << " with queue ability";
            if (queueFamiliyProperties.at(i).queueFlags | VK_QUEUE_GRAPHICS_BIT) {
                std::cout << " VK_QUEUE_GRAPHICS_BIT ";
            }
            if (queueFamiliyProperties.at(i).queueFlags | VK_QUEUE_TRANSFER_BIT) {
                std::cout << " VK_QUEUE_TRANSFER_BIT ";
            }
            #endif
            std::cout << std::endl;

            if (queueFamiliyProperties.at(i).queueFlags | VK_QUEUE_GRAPHICS_BIT
                &&
                queueFamiliyProperties.at(i).queueFlags | VK_QUEUE_TRANSFER_BIT) {
                queueFamilySelection = i;
                break;
            }
        }

#ifdef PRINT_INFO
        std::cout << "Selected QueueFamily: " << queueFamilySelection << std::endl;
#endif

        return queueFamilySelection;
    }


    typedef struct SurfaceInfo_ {
        VkFormat imageFormat;
        VkColorSpaceKHR imageColorSpace;
        VkExtent2D imageExtent;
        uint32_t imageArrayLayerCount;
        VkImageUsageFlags imageUsage;
        VkSharingMode imageSharingMode;

        VkSurfaceTransformFlagBitsKHR preTransform;
    } SurfaceInfo;

    inline SurfaceInfo getSurfaceInfo(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        uint32_t surfaceFormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr);
        std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());
        VkColorSpaceKHR colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        for (uint32_t i = 0; i < surfaceFormatCount; i += 1) {
            if (surfaceFormats.at(i).format == VK_FORMAT_R8G8B8A8_SRGB) {
                colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
                break;
            }
            if (i == surfaceFormatCount - 1) {
                std::cerr << "This device can't use VK_FORMAT_R8G8B8A8_SRGB" << std::endl;
            }
        }




        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vk::Extent2D extent = vk::Extent2D(1, 1);
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            extent.width = capabilities.currentExtent.width;
            extent.height = capabilities.currentExtent.height;
        }

        return SurfaceInfo {
        .imageFormat = VK_FORMAT_R8G8B8A8_SRGB,
        .imageColorSpace = colorSpace,
        .imageExtent = extent,
        .imageArrayLayerCount = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = sharingMode,
        .preTransform = capabilities.currentTransform,
        };
    }

    inline std::vector<uint32_t> readShaderCodeUINT32(const std::string &shaderFileName) {
        std::vector<uint32_t> buffer{};

        std::ifstream file(shaderFileName, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            return buffer;
        }

        // find what the size of the file is by looking up the location of the cursor
        // because the cursor is at the end, it gives the size directly in bytes
        const uint32_t fileSize = file.tellg();

        // spirv expects the buffer to be on uint32, so make sure to reserve int
        // vector big enough for the entire file
        //std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

        buffer.resize(fileSize / sizeof(uint32_t));

        // put file cursor at beginning
        file.seekg(0);
        // load the entire file into the buffer
        file.read(reinterpret_cast<char *>(buffer.data()), fileSize);

        // now that the file is loaded into the buffer, we can close it
        file.close();

        return buffer;
    }
}