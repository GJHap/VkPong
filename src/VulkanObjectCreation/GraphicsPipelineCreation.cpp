#include "GraphicsPipelineCreation.hpp"
#include "../Structs/VertexData.hpp"

#include <fstream>

namespace vkPong
{
	static std::vector<char> getShaderCode(const std::string& spirvPath)
	{
		std::ifstream spirvFileStream(spirvPath, std::ios::binary);

		std::vector<char> spirvCode;
		if (spirvFileStream.is_open())
		{
			spirvCode = std::vector<char>(std::istreambuf_iterator<char>(spirvFileStream), std::istreambuf_iterator<char>());
		}

		return spirvCode;
	}

	static VkShaderModule createShaderModule(const vk::Device& logicalDevice, const std::string& spirvPath)
	{
		std::vector<char> shaderCode = getShaderCode(spirvPath);
		vk::ShaderModuleCreateInfo shaderModuleCreateInfo;
		shaderModuleCreateInfo.setCodeSize(shaderCode.size());
		shaderModuleCreateInfo.setPCode(reinterpret_cast<const uint32_t*>(shaderCode.data()));

		return logicalDevice.createShaderModule(shaderModuleCreateInfo);
	}

	static vk::PipelineShaderStageCreateInfo createPipelineShaderStageCreateInfo(const vk::Device logicalDevice, const vk::ShaderStageFlagBits& stage, const std::string& spirvPath)
	{
		vk::PipelineShaderStageCreateInfo shaderStageCreateInfo;
		shaderStageCreateInfo.setStage(stage);
		shaderStageCreateInfo.setModule(createShaderModule(logicalDevice, spirvPath));
		shaderStageCreateInfo.setPName("main");

		return shaderStageCreateInfo;
	}

	vk::Pipeline createGraphicsPipeline(const vk::PipelineLayout& graphicsLayout, const vk::Device& logicalDevice, const vk::RenderPass& renderPass, const vk::Extent2D& extent)
	{
		vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;
		pipelineInputAssemblyStateCreateInfo.setTopology(vk::PrimitiveTopology::eTriangleList);

		vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo;
		pipelineRasterizationStateCreateInfo.setFrontFace(vk::FrontFace::eClockwise);
		pipelineRasterizationStateCreateInfo.setPolygonMode(vk::PolygonMode::eFill);
		pipelineRasterizationStateCreateInfo.setLineWidth(1.0f);

		vk::VertexInputBindingDescription vertexInputBindingDescription;
		vertexInputBindingDescription.setBinding(0);
		vertexInputBindingDescription.setStride(sizeof(VertexData));

		vk::VertexInputAttributeDescription vertexInputAttributeDescription1;
		vertexInputAttributeDescription1.setBinding(vertexInputBindingDescription.binding);
		vertexInputAttributeDescription1.setFormat(vk::Format::eR32G32B32Sfloat);
		vertexInputAttributeDescription1.setLocation(0);
		vertexInputAttributeDescription1.setOffset(VertexData::positionOffset());

		vk::VertexInputAttributeDescription vertexInputAttributeDescription2;
		vertexInputAttributeDescription2.setBinding(vertexInputBindingDescription.binding);
		vertexInputAttributeDescription2.setFormat(vk::Format::eR32G32B32Sfloat);
		vertexInputAttributeDescription2.setLocation(1);
		vertexInputAttributeDescription2.setOffset(VertexData::colorOffset());

		std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions{ vertexInputAttributeDescription1, vertexInputAttributeDescription2 };

		vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
		pipelineVertexInputStateCreateInfo.setVertexBindingDescriptionCount(1);
		pipelineVertexInputStateCreateInfo.setPVertexBindingDescriptions(&vertexInputBindingDescription);
		pipelineVertexInputStateCreateInfo.setVertexAttributeDescriptionCount(static_cast<uint32_t>(vertexInputAttributeDescriptions.size()));
		pipelineVertexInputStateCreateInfo.setPVertexAttributeDescriptions(vertexInputAttributeDescriptions.data());

		std::vector<vk::PipelineShaderStageCreateInfo> shaderCreateInfos
		{
			createPipelineShaderStageCreateInfo(logicalDevice, vk::ShaderStageFlagBits::eVertex, "./shaders/vertex/shader.spv"),
			createPipelineShaderStageCreateInfo(logicalDevice, vk::ShaderStageFlagBits::eFragment, "./shaders/fragment/shader.spv"),
		};

		vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;

		vk::Rect2D scissor;
		scissor.setExtent(extent);

		vk::Viewport viewport;
		viewport.setWidth(static_cast<float>(extent.width));
		viewport.setHeight(static_cast<float>(extent.height));
		viewport.setMaxDepth(1.0f);

		vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;
		pipelineViewportStateCreateInfo.setViewportCount(1);
		pipelineViewportStateCreateInfo.setPViewports(&viewport);
		pipelineViewportStateCreateInfo.setScissorCount(1);
		pipelineViewportStateCreateInfo.setPScissors(&scissor);

		vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
		pipelineColorBlendAttachmentState.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
		pipelineColorBlendAttachmentState.setBlendEnable(1);
		pipelineColorBlendAttachmentState.setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha);
		pipelineColorBlendAttachmentState.setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha);
		pipelineColorBlendAttachmentState.setSrcAlphaBlendFactor(vk::BlendFactor::eOne);

		vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo;
		pipelineColorBlendStateCreateInfo.setAttachmentCount(1);
		pipelineColorBlendStateCreateInfo.setPAttachments(&pipelineColorBlendAttachmentState);

		vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
		graphicsPipelineCreateInfo.setLayout(graphicsLayout);
		graphicsPipelineCreateInfo.setPColorBlendState(&pipelineColorBlendStateCreateInfo);
		graphicsPipelineCreateInfo.setPInputAssemblyState(&pipelineInputAssemblyStateCreateInfo);
		graphicsPipelineCreateInfo.setPMultisampleState(&pipelineMultisampleStateCreateInfo);
		graphicsPipelineCreateInfo.setPRasterizationState(&pipelineRasterizationStateCreateInfo);
		graphicsPipelineCreateInfo.setPStages(shaderCreateInfos.data());
		graphicsPipelineCreateInfo.setStageCount(static_cast<uint32_t>(shaderCreateInfos.size()));
		graphicsPipelineCreateInfo.setPVertexInputState(&pipelineVertexInputStateCreateInfo);
		graphicsPipelineCreateInfo.setPViewportState(&pipelineViewportStateCreateInfo);
		graphicsPipelineCreateInfo.setRenderPass(renderPass);

		vk::Pipeline graphicsPipeline = logicalDevice.createGraphicsPipeline(vk::PipelineCache(), graphicsPipelineCreateInfo);

		for (VkPipelineShaderStageCreateInfo shaderCreateInfo : shaderCreateInfos)
		{
			logicalDevice.destroyShaderModule(shaderCreateInfo.module);
		}

		return graphicsPipeline;
	}
}