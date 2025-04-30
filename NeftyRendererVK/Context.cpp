
#include "NeftyRendererVK.h"

namespace NEFTY {
    std::vector<const char*> device_extensions = {
        R"(VK_KHR_swapchain)",
        R"(VK_KHR_dynamic_rendering)",
        R"(VK_EXT_descriptor_buffer)"
    };

    void beforeRender();

    NeftyContext::NeftyContext() {

        if (vkut::checkValidationLayerSupport() == vk::True) {
            instanceLayerRequestList.push_back("VK_LAYER_KHRONOS_validation");
        }

        auto appInfo = vk::ApplicationInfo(APP_NAME, 0.0, APP_NAME, 0.0,
#ifdef __APPLE__
            VK_API_VERSION_1_3
#else
            VK_API_VERSION_1_3
#endif
            );
        auto instCreateInfo = vk::InstanceCreateInfo(
            vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR,
            &appInfo,
            static_cast<uint32_t>(instanceLayerRequestList.size()),
            instanceLayerRequestList.data(),
            static_cast<uint32_t>(instanceExtensionRequestList.size()),
            instanceExtensionRequestList.data()
            );

        instanceUnique = vk::createInstanceUnique(instCreateInfo);

        if (instanceUnique.get() == VK_NULL_HANDLE) {
            std::cerr << "Failed to create instance" << std::endl;
            exit(EXIT_FAILURE);
        }

        auto enumerationPhysicalDevice = instanceUnique->enumeratePhysicalDevices();

        physicalDevicesUnique->insert(physicalDevicesUnique->begin(), enumerationPhysicalDevice.begin(), enumerationPhysicalDevice.end());

        if (physicalDevicesUnique->empty()) {
            std::cerr << "No device available." << std::endl;
        }

#ifdef PRINT_INFO

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevicesUnique->at(DEVICE_SELECTION), &deviceProperties);
        std::cout << "Physical device selected: " << "number: " << DEVICE_SELECTION  << std::endl << "device name:" << deviceProperties.deviceName << std::endl;

#endif

        const uint32_t queueFamilySelection = vkut::autoSelectQueueFamily(physicalDevicesUnique->at(DEVICE_SELECTION), DEVICE_SELECTION);

        const vk::PhysicalDevice pd = physicalDevicesUnique->at(DEVICE_SELECTION);

        /*** BAD WAY
        VkPhysicalDeviceVulkan14Features deviceFeaturesVK14 {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES};
        VkPhysicalDeviceVulkan13Features deviceFeaturesVK13{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
        VkPhysicalDeviceVulkan12Features deviceFeaturesVK12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
        VkPhysicalDeviceVulkan11Features deviceFeaturesVK11{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES };
        vk::PhysicalDeviceFeatures2 physicalDeviceFeatures2 { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };

        pd.getFeatures2(&physicalDeviceFeatures2);


        //deviceFeaturesVK13.dynamicRendering = VK_TRUE;
        //deviceFeaturesVK13.synchronization2 = VK_TRUE;
        deviceFeaturesVK12.bufferDeviceAddress = VK_TRUE;
        deviceFeaturesVK12.descriptorIndexing = VK_TRUE;


        deviceFeaturesVK13.pNext = &deviceFeaturesVK14;
        deviceFeaturesVK12.pNext = &deviceFeaturesVK13;
        deviceFeaturesVK11.pNext = &deviceFeaturesVK12;
        physicalDeviceFeatures2.pNext = &deviceFeaturesVK11;
        physicalDeviceFeatures2.features.fillModeNonSolid = VK_TRUE;
        */

        auto physicalDeviceFeatures = pd.getFeatures2();
        auto dynamicFeatures = vk::PhysicalDeviceDynamicRenderingFeaturesKHR(1);

        physicalDeviceFeatures.setPNext(&dynamicFeatures);

        const auto queueCreateInfo = vk::DeviceQueueCreateInfo({}, queueFamilySelection, 1, &queuePriority);
        auto deviceCreateInfo = vk::DeviceCreateInfo({},
            1, &queueCreateInfo,
            0, nullptr,
            device_extensions.size(), device_extensions.data());
        deviceCreateInfo.setPNext(&physicalDeviceFeatures);

        deviceUnique = pd.createDeviceUnique(deviceCreateInfo);

        if (deviceUnique.get() == VK_NULL_HANDLE) {
            std::cerr << "Cannot create logical device." << std::endl;
        }

        queue = deviceUnique->getQueue(queueFamilySelection, 0);


    }

    void beforeRender() {
    }

    NeftyContext::~NeftyContext() {


        for (const auto imageView: PresentationUnit.swapChainImageViews) {
            vkDestroyImageView(deviceUnique.get(), imageView, nullptr);
        }

        for (const auto image: PresentationUnit.swapChainImages) {
            vkDestroyImage(deviceUnique.get(), image, nullptr);
        }

        vkDestroyFence(deviceUnique.get(), synchronizer.fenceUnique.get(), nullptr);

        vkDestroySwapchainKHR(deviceUnique.get(), PresentationUnit.swapChaineUnique.get(), nullptr);

        vkDestroyCommandPool(deviceUnique.get(), commandPoolUnique.get(), nullptr);
        vkDestroyDevice(deviceUnique.get(), nullptr);


        std::cout << "Nutshell says goodbye~" << std::endl; // if this doesn't happen, you are doing destroy in wrong way.
    }
}
