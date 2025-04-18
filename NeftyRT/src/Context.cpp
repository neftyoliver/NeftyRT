#include <vulkan/vulkan.hpp>
#include <nefty_rt.h>

namespace NEFTY {
    const char * deviceExtensions[255] = {
        R"(VK_KHR_swapchain)"
    };

    void beforeRender();

    inline VkContext_::VkContext_() {
        SDL_Init(SDL_INIT_EVERYTHING);
        PresentationUnit.window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WIDTH, HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);

        auto appInfo = vk::ApplicationInfo(APP_NAME, 0.0, APP_NAME, 0.0, VK_API_VERSION_1_4);
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


        const auto queueCreateInfo = vk::DeviceQueueCreateInfo({}, queueFamilySelection, 1, &queuePriority);
        const auto deviceCreateInfo = vk::DeviceCreateInfo({}, 1, &queueCreateInfo, 0, nullptr, 0, deviceExtensions);
        const vk::PhysicalDevice pd = physicalDevicesUnique->at(DEVICE_SELECTION);
        deviceUnique = pd.createDeviceUnique(deviceCreateInfo);
        if (deviceUnique.get() == VK_NULL_HANDLE) {
            std::cerr << "Cannot create logical device." << std::endl;
        }

        queue = deviceUnique->getQueue(queueFamilySelection, 1);


    }


    inline void VkContext_::programLoop() const {
    }

    inline VkContext_::~VkContext_() {
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