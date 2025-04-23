#include <vulkan//vulkan.hpp>
#include <GLFW/glfw3.h>

#include "vulkan_utility.h"
#include "ExecutablesReference.h"
#include <iostream>



struct GraphicsBasic : NEFTY::ExecutablesReference {
    typedef ExecutablesReference SUPER;



    vk::ShaderModule vertShader;
    vk::ShaderModule fragShader;

    vk::RenderPass renderPass;
    vk::SubpassDescription subpass;

    vk::UniquePipeline pipline = vk::UniquePipeline();
    vk::PipelineLayout pipelineLayout;
    vk::DescriptorSetLayout descriptorSetLayout;
    vk::DescriptorSet descriptorSet;
    vk::DescriptorSetLayout descriptorLayout;

    GraphicsBasic() {
        std::cout << "Creating graphics object..." << std::endl;

        vk::PipelineRenderingCreateInfo renderingInfo {};

        auto vecVertexShaderCode = vkut::readShaderCodeUINT32("GraphicsBasicFragment.frag");
        auto vecFragmentShaderCodes = vkut::readShaderCodeUINT32("GraphicsBasicFragment.frag");

        auto vertexShaderModule = context.deviceUnique->createShaderModuleUnique(
            vk::ShaderModuleCreateInfo(
                {},
                ,
                ,
                    nullptr
                )
        );

        auto fragmentShaderModule = context.deviceUnique->createShaderModuleUnique(
            vk::ShaderModuleCreateInfo(
                {},
                    GRAPHICS_BASIC::fragmentCode.size(),
                    GRAPHICS_BASIC::fragmentCode.c_str(),
                    nullptr
                )
        );

        constexpr uint32_t SHADER_COUNT = 2;

        /**
        *: pNext{ pNext_ }
      , flags{ flags_ }
      , stage{ stage_ }
      , module{ module_ }
      , pName{ pName_ }
      , pSpecializationInfo{ pSpecializationInfo_ }
         */

        vk::PipelineShaderStageCreateInfo vertexShaderStages {};
        vertexShaderStages.setModule(vertexShaderModule.get());
        vertexShaderStages.setStage(vk::ShaderStageFlagBits::eVertex);
        vertexShaderStages.setPName("main");

        vk::PipelineShaderStageCreateInfo fragmentShaderStages {};
        fragmentShaderStages.setModule(fragmentShaderModule.get());
        fragmentShaderStages.setStage(vk::ShaderStageFlagBits::eFragment);
        fragmentShaderStages.setPName("main");

        vk::PipelineShaderStageCreateInfo shaderStages[SHADER_COUNT] { vertexShaderStages, fragmentShaderStages};

        vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo {};

        constexpr uint32_t bindingCount = 2;
        constexpr vk::DescriptorSetLayoutBinding bindings[bindingCount] = {
            vk::DescriptorSetLayoutBinding {
                0,
                vk::DescriptorType::eStorageBuffer,
                1,
                vk::ShaderStageFlagBits::eVertex,
            },

            vk::DescriptorSetLayoutBinding {
                0,
                vk::DescriptorType::eUniformBuffer,
                3,
                vk::ShaderStageFlagBits::eFragment,
            }
        };
        descriptorSetLayoutCreateInfo.setBindingCount(bindingCount);
        descriptorSetLayoutCreateInfo.setBindings(bindings);
        descriptorSetLayoutCreateInfo.setFlags(vk::DescriptorSetLayoutCreateFlagBits::eDescriptorBufferEXT);
        vk::DescriptorSetLayout descriptorSetLayout = context.deviceUnique->createDescriptorSetLayout(descriptorSetLayoutCreateInfo);

        vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo {}; //Don't need if not doing any vertex input
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo {};
        inputAssemblyStateCreateInfo.setTopology(vk::PrimitiveTopology::eLineStrip);
        inputAssemblyStateCreateInfo.setPrimitiveRestartEnable(VK_FALSE);

        vk::PipelineTessellationStateCreateInfo tessellation_state_create_info {};

        vk::PipelineViewportStateCreateInfo viewportStateCreateInfo {};
        viewportStateCreateInfo.setViewportCount(1);
        viewportStateCreateInfo.setScissorCount(1);

        vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo {};
        rasterizationStateCreateInfo.setPolygonMode(vk::PolygonMode::eLine);
        rasterizationStateCreateInfo.setLineWidth(2.0f);
        rasterizationStateCreateInfo.setCullMode(vk::CullModeFlagBits::eFront);
        rasterizationStateCreateInfo.setFrontFace(vk::FrontFace::eCounterClockwise);

        vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo {};
        colorBlendStateCreateInfo.setLogicOpEnable(VK_FALSE);
        colorBlendStateCreateInfo.setLogicOp(vk::LogicOp::eCopy);
        colorBlendStateCreateInfo.setAttachmentCount(1);
        vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendStateCreateInfo.setAttachments(colorBlendAttachment);

        vk::PipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo {}; //disabled all depth operation
        depthStencilStateCreateInfo.depthTestEnable = VK_FALSE;
        depthStencilStateCreateInfo.depthWriteEnable = VK_FALSE;
        depthStencilStateCreateInfo.depthCompareOp = vk::CompareOp::eNever;
        depthStencilStateCreateInfo.setDepthBoundsTestEnable(VK_FALSE);
        depthStencilStateCreateInfo.setFront({});
        depthStencilStateCreateInfo.setBack({});
        depthStencilStateCreateInfo.setMinDepthBounds(0.0f);
        depthStencilStateCreateInfo.setMaxDepthBounds(1.0f);

        vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo {};
        pipelineLayoutCreateInfo.setSetLayoutCount(1);
        pipelineLayoutCreateInfo.setPSetLayouts(nullptr);

        vk::GraphicsPipelineCreateInfo pipelineCreateInfo {};
        pipelineCreateInfo.setPNext(&renderingInfo);
        pipelineCreateInfo.setStageCount(SHADER_COUNT);
        pipelineCreateInfo.setStages(shaderStages);
        pipelineCreateInfo.setPVertexInputState(&vertexInputStateCreateInfo);
        pipelineCreateInfo.setPInputAssemblyState(&inputAssemblyStateCreateInfo);
        pipelineCreateInfo.setPTessellationState(&tessellation_state_create_info); //not using
        pipelineCreateInfo.setPViewportState(&viewportStateCreateInfo);
        pipelineCreateInfo.setPRasterizationState(&rasterizationStateCreateInfo);
        pipelineCreateInfo.setPColorBlendState(&colorBlendStateCreateInfo);
        pipelineCreateInfo.setPDepthStencilState(&depthStencilStateCreateInfo);
        pipelineCreateInfo.setLayout(pipelineLayout);
        pipelineCreateInfo.setRenderPass(renderPass);


        auto graphicsPipeline = context.deviceUnique->createGraphicsPipelineUnique(VK_NULL_HANDLE, pipelineCreateInfo);
        pipline = std::move(graphicsPipeline.value);
    }



    ~GraphicsBasic() {

        std::cout << "Destroying graphics object..." << std::endl;
        //this->context.deviceUnique->destroyShaderModule(vertShader);
        //this->context.deviceUnique->destroyShaderModule(fragShader);
    }
};

void NEFTY::ExecutablesReference::update(NEFTY::NeftyContext * context, GLFWwindow* window) {
    glfwPollEvents();
}

void NEFTY::ExecutablesReference::render(NEFTY::NeftyContext * context) {
    /*
    context->commandBufferUnique->begin(vk::CommandBufferBeginInfo {
        vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
        nullptr,
        nullptr
    });

    context->commandBufferUnique->end();
    */
}




int main(int argc, const char * argv[]) {
    GraphicsBasic basicRaytracing = GraphicsBasic();

    NEFTY::run(&basicRaytracing);

    return 0;
}