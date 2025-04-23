#include <GLFW/glfw3.h>

#include "vulkan_utility.h"
#include "ExecutablesReference.h"
#include <iostream>



struct GraphicsBasic : nefty::ExecutablesReference {
    typedef ExecutablesReference SUPER;



    vk::ShaderModule vertShader;
    vk::ShaderModule fragShader;

    vk::RenderPass renderPass;
    vk::SubpassDescription subpass;

    vk::UniquePipeline pipline;
    vk::PipelineLayout pipelineLayout;
    vk::DescriptorSetLayout descriptorSetLayout;
    vk::DescriptorSet descriptorSet;
    vk::DescriptorSetLayout descriptorLayout;

    GraphicsBasic() {
        std::cout << "Creating graphics object..." << std::endl;

        vk::PipelineRenderingCreateInfo renderingInfo {};

        auto vecVertexShaderCode = vkut::readShaderCodeUINT32("GraphicsBasicFragment.spv");
        auto vecFragmentShaderCodes = vkut::readShaderCodeUINT32("GraphicsBasicFragment.spv");

        auto vertexShaderModule = context.deviceUnique->createShaderModuleUnique(
            vk::ShaderModuleCreateInfo(
                {},
                vecVertexShaderCode.size() * 4,
                vecVertexShaderCode.data(),
                    nullptr
                )
        );

        auto fragmentShaderModule = context.deviceUnique->createShaderModuleUnique(
            vk::ShaderModuleCreateInfo(
                {},
                    vecFragmentShaderCodes.size() * 4,
                    vecFragmentShaderCodes.data(),
                    nullptr
                )
        );



        constexpr uint32_t SHADER_COUNT = 2;

        auto vertexShaderStages = vk::PipelineShaderStageCreateInfo {};
        vertexShaderStages.setModule(vertexShaderModule.get());
        vertexShaderStages.setStage(vk::ShaderStageFlagBits::eVertex);
        vertexShaderStages.setPName("main");

        auto fragmentShaderStages = vk::PipelineShaderStageCreateInfo{};
        fragmentShaderStages.setModule(fragmentShaderModule.get());
        fragmentShaderStages.setStage(vk::ShaderStageFlagBits::eFragment);
        fragmentShaderStages.setPName("main");

        vk::PipelineShaderStageCreateInfo shaderStages[SHADER_COUNT] { vertexShaderStages, fragmentShaderStages};

        auto descriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo {};

        constexpr uint32_t bindingCount = 2;
        constexpr vk::DescriptorSetLayoutBinding bindings[bindingCount] = {
            vk::DescriptorSetLayoutBinding {
                0,
                vk::DescriptorType::eStorageBuffer,
                1,
                vk::ShaderStageFlagBits::eVertex,
            },

            vk::DescriptorSetLayoutBinding {
                1,
                vk::DescriptorType::eUniformBuffer,
                3,
                vk::ShaderStageFlagBits::eFragment,
            }
        };

        descriptorSetLayoutCreateInfo.setBindingCount(bindingCount);
        descriptorSetLayoutCreateInfo.setBindings(bindings);
        descriptorSetLayoutCreateInfo.setFlags(vk::DescriptorSetLayoutCreateFlagBits::eDescriptorBufferEXT);
        vk::DescriptorSetLayout descriptorSetLayout = context.deviceUnique->createDescriptorSetLayout(descriptorSetLayoutCreateInfo);

        auto vertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo {}; //Don't need if not doing any vertex input
        auto inputAssemblyStateCreateInfo = vk::PipelineInputAssemblyStateCreateInfo {};
        inputAssemblyStateCreateInfo.setTopology(vk::PrimitiveTopology::eLineStrip);
        inputAssemblyStateCreateInfo.setPrimitiveRestartEnable(VK_FALSE);

        auto tessellation_state_create_info = vk::PipelineTessellationStateCreateInfo {}; //Blank for now...

        auto viewportStateCreateInfo = vk::PipelineViewportStateCreateInfo {};
        viewportStateCreateInfo.setViewportCount(1);
        viewportStateCreateInfo.setScissorCount(1);

        auto rasterizationStateCreateInfo = vk::PipelineRasterizationStateCreateInfo {};
        rasterizationStateCreateInfo.setPolygonMode(vk::PolygonMode::eLine);
        rasterizationStateCreateInfo.setLineWidth(2.0f);
        rasterizationStateCreateInfo.setCullMode(vk::CullModeFlagBits::eFront);
        rasterizationStateCreateInfo.setFrontFace(vk::FrontFace::eCounterClockwise);

        auto colorBlendStateCreateInfo = vk::PipelineColorBlendStateCreateInfo {};
        colorBlendStateCreateInfo.setLogicOpEnable(VK_FALSE);
        colorBlendStateCreateInfo.setLogicOp(vk::LogicOp::eCopy);
        colorBlendStateCreateInfo.setAttachmentCount(1);

        auto colorBlendAttachment = vk::PipelineColorBlendAttachmentState {};
        colorBlendStateCreateInfo.setAttachments(colorBlendAttachment);

        auto depthStencilStateCreateInfo = vk::PipelineDepthStencilStateCreateInfo {}; //disabled all depth operation
        depthStencilStateCreateInfo.depthTestEnable = VK_FALSE;
        depthStencilStateCreateInfo.depthWriteEnable = VK_FALSE;
        depthStencilStateCreateInfo.depthCompareOp = vk::CompareOp::eNever;
        depthStencilStateCreateInfo.setDepthBoundsTestEnable(VK_FALSE);
        depthStencilStateCreateInfo.setFront({});
        depthStencilStateCreateInfo.setBack({});
        depthStencilStateCreateInfo.setMinDepthBounds(0.0f);
        depthStencilStateCreateInfo.setMaxDepthBounds(1.0f);

        auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.setSetLayoutCount(0);
        pipelineLayoutCreateInfo.setPSetLayouts(nullptr);
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.setPSetLayouts(nullptr);

        auto dynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo {};
        std::vector<vk::DynamicState> dynamicStages {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor,
        };
        dynamicStateCreateInfo.setPDynamicStates(dynamicStages.data());
        dynamicStateCreateInfo.setDynamicStateCount(dynamicStages.size());

        auto graphicsPipelineCreateInfo = vk::GraphicsPipelineCreateInfo {};
        //graphicsPipelineCreateInfo.setPNext(&renderingInfo);
        graphicsPipelineCreateInfo.setStageCount(SHADER_COUNT);
        graphicsPipelineCreateInfo.setStages(shaderStages);
        graphicsPipelineCreateInfo.setPVertexInputState(&vertexInputStateCreateInfo);
        graphicsPipelineCreateInfo.setPInputAssemblyState(&inputAssemblyStateCreateInfo);
        graphicsPipelineCreateInfo.setPTessellationState(&tessellation_state_create_info); //not using
        graphicsPipelineCreateInfo.setPViewportState(&viewportStateCreateInfo);
        graphicsPipelineCreateInfo.setPRasterizationState(&rasterizationStateCreateInfo);
        graphicsPipelineCreateInfo.setPColorBlendState(&colorBlendStateCreateInfo);
        graphicsPipelineCreateInfo.setPDepthStencilState(&depthStencilStateCreateInfo);
        graphicsPipelineCreateInfo.setLayout(pipelineLayout);
        graphicsPipelineCreateInfo.setRenderPass(VK_NULL_HANDLE);
        graphicsPipelineCreateInfo.setPDynamicState(&dynamicStateCreateInfo);



        /**
        *pipeline_create.colorAttachmentCount    = 1;
pipeline_create.pColorAttachmentFormats = &color_rendering_format;
pipeline_create.depthAttachmentFormat   = depth_format;
pipeline_create.stencilAttachmentFormat = depth_format;

        vk::Format colorAttachmentFormat = vk::Format::eR8G8B8A8Srgb;

        vk::PipelineRenderingCreateInfoKHR dynamicRenderingPiplineCreateInfo {};
        dynamicRenderingPiplineCreateInfo.setColorAttachmentCount(1);
        dynamicRenderingPiplineCreateInfo.setColorAttachmentFormats(colorAttachmentFormat);
        //graphicsPipelineCreateInfo.setPNext(&dynamicRenderingPiplineCreateInfo);

        */



        auto uniqueGraphicsPipeline = context.deviceUnique->createGraphicsPipelineUnique(VK_NULL_HANDLE, graphicsPipelineCreateInfo).value;
        pipline = std::move(uniqueGraphicsPipeline);

        std::cout << "Finished creating graphics object!" << std::endl;
    }



    ~GraphicsBasic() {

        std::cout << "Destroying graphics object..." << std::endl;
        //this->context.deviceUnique->destroyShaderModule(vertShader);
        //this->context.deviceUnique->destroyShaderModule(fragShader);
    }
};

void nefty::ExecutablesReference::update(nefty::NeftyContext * context, GLFWwindow* window) {
    glfwPollEvents();
}

void nefty::ExecutablesReference::render(nefty::NeftyContext * context) {
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

    nefty::run(&basicRaytracing);

    return 0;
}