#pragma once


#include <vulkan/vulkan.hpp>
#include <memory>

#include "vulkan_utility.h"


#define DEVICE_SELECTION 0
#define QUEUE_PRIORITY 1.0f
#define PRINT_INFO

#define APP_NAME "NeftyRendererVK"

#define WIDTH 1200
#define HEIGHT 800


#ifdef NDEBUG
    constexpr bool enableValidationLayers = false;
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
    typedef struct NeftyContext {
        struct {
            vk::UniqueSurfaceKHR surfaceUnique;
            vk::UniqueSwapchainKHR swapChaineUnique;
            std::vector<VkImage> swapChainImages{};
            std::vector<VkImageView> swapChainImageViews{};
            std::vector<VkFramebuffer> framebuffer{};
        } PresentationUnit;

        std::vector<const char *> instanceLayerRequestList {
        };
        std::vector<const char *> instanceExtensionRequestList = {
            R"(VK_KHR_surface)",
            R"(VK_KHR_portability_enumeration)"

#ifdef __APPLE__
            //"VK_KHR_portability_subset"
#endif
        };

        vk::UniqueInstance instanceUnique;

        std::unique_ptr<std::vector<VkPhysicalDevice>> physicalDevicesUnique = std::make_unique<std::vector<VkPhysicalDevice>>();
        const float queuePriority = 1.0;
        vk::UniqueDevice deviceUnique;
        vk::Queue queue;
        vk::UniqueCommandPool commandPoolUnique;
        vk::UniqueCommandBuffer commandBufferUnique;

        Synchronizer synchronizer = {};

        NeftyContext();


        ~NeftyContext();

    } NeftyContext;


    namespace RENDER_SUBJECT {
        typedef struct f4_ {
            f4_(const float px, float const py, const float pz, const float pw): x(px), y(py), z(pz), w(pw) {}

            union {
                struct {
                    float x;
                    float y;
                    float z;
                    float w;
                };
                float data[4] { x, y, z, w };
            };

        } float4_u;
        typedef struct f3_ {
            f3_(const float px, const float py, const float pz): x(px), y(py), z(pz) {}

            union {
                struct {
                    float x;
                    float y;
                    float z;
                };
                float data[3] { x, y, z };
            };

        } float3_u;
        typedef struct f2_ {
            f2_(const float px, const float py): x(px), y(py){}

            union {
                struct {
                    float x;
                    float y;
                };
                float data[2] { x, y };
            };

        } float2_u;

        /*
         *
         * DefaultRenderPass
         *
         * GraphicsShaderModules
         * RaytracingShaderModules
         *
         * UniformBufferObject
         * StorageBufferObject
         * Image
         * ImageView
         *
         * just variables
         */


    }

    namespace RENDER_UNIT {
        /***
         *
         * Rendering units are the objective unit that dose rendering operation with a subjects.
         * it's a unit because it operates itself when the method is called.
         * while the subjects are just a data, units are the ones using the date in certain manners.
         *
         * The Units:
         *
         * CommandOperatorUnique
         * CommandRecorderUnique
         * DynamicGraphicsPiplineUnique WORKING!
         * GraphicsPiplineUnique
         * RayTracingBindTable
         * RayTracingPiplineUnique
         *
         */


        typedef struct DynamicGraphicsPiplineUnique_ {
        } DynamicGraphicsPiplineUnique;
        /***
         *
         *  Graphics pipline info builder.
         *  you can use builder pattern for the graphics pipline building.
         *  for many of reasons the build().get(std::unique_ptr) method will give you unique ptr of the object.
         *
         */
        struct GraphicsPiplineCreateInfoBuilder {
            vk::PipelineRenderingCreateInfo dynamicRenderingInfo;

            GraphicsPiplineCreateInfoBuilder build();
            std::unique_ptr<DynamicGraphicsPiplineUnique> get();
        };

    }

}


