#pragma once

#include <iostream>
#include <memory>
#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>

#include "../include/vulkan_utility.h"

#define DEVICE_SELECTION 0
#define QUEUE_PRIORITY 1.0f
#define PRINT_INFO

#define APP_NAME "NeftyRT"

#define WIDTH 1200
#define HEIGHT 800


#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif



namespace NEFTY {
    typedef struct Synchronizer_ {
        vk::UniqueFence fenceUnique;
        vk::UniqueSemaphore semaphoreUnique;
    } Synchronizer;

    /**
     * Very simple Vulkan instance context with some device info and the command pool.
     **/
    typedef struct VkContext_ {
        struct {
            SDL_Window* window;
            vk::UniqueSurfaceKHR surfaceUnique;
            vk::UniqueSwapchainKHR swapChaineUnique;
            std::vector<VkImage> swapChainImages{};
            std::vector<VkImageView> swapChainImageViews{};
            std::vector<VkFramebuffer> framebuffer{};
        } PresentationUnit;

        std::vector<const char *> instanceLayerRequestList {
        };
        std::vector<const char *> instanceExtensionRequestList = {
            "VK_KHR_portability_enumeration",

#ifdef __APPLE__
            "VK_KHR_portability_subset"
#endif
        };

        vk::UniqueInstance instanceUnique;

        std::unique_ptr<std::vector<VkPhysicalDevice>> physicalDevicesUnique{};
        const float queuePriority = 1.0;
        vk::UniqueDevice deviceUnique;
        vk::Queue queue;
        vk::UniqueCommandPool commandPoolUnique;
        vk::UniqueCommandBuffer commandBufferUnique;

        Synchronizer synchronizer = {};

        VkContext_();

        /**
         * After call this function the program will be started and loops.
         */
        void programLoop() const;


        ~VkContext_();

    } VkContext;


}


