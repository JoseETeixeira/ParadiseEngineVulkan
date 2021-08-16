#pragma once
#ifndef MESH_SYSTEM_H
#define MESH_SYSTEM_H

#include "../System.hpp"
#include "../../vulkan_example_base/vulkan_example_base.h"
#include "../../gui/gui.hpp"
#include "../components/Transform.hpp"
#include "../components/Camera.hpp"
#include "../Coordinator.hpp"


#include "../../vulkan_gltf_model/vulkan_gltf_model.hpp"

class Event;

extern Coordinator gCoordinator;

class MeshSystem : public System
{
public:
    VulkanExampleBase *example;
    ImGUI *imGui = nullptr;
	VulkanglTFModel glTFModel;
	struct DescriptorSetLayouts {
		VkDescriptorSetLayout matrices;
		VkDescriptorSetLayout textures;
	} descriptorSetLayouts;
	struct ShaderData {
		vks::Buffer buffer;
		struct Values {
			glm::mat4   projection;
			glm::mat4  model;
			glm::vec4 lightPos = glm::vec4(5.0f, 5.0f, -5.0f, 1.0f);
		} values;
	} shaderData;
	
	struct Pipelines {
		VkPipeline solid;
		VkPipeline wireframe = VK_NULL_HANDLE;
	} pipelines;
	VkPipelineLayout pipelineLayout;
	VkDescriptorSet descriptorSet;
	VkDevice device;

	Entity mesh;

	void Init(VulkanExampleBase *ex, std::string path){
		example = ex;
		mesh = gCoordinator.CreateEntity();

		gCoordinator.AddComponent(
			mesh,
			Transform{
				.position = Vec3(0.0f, 0.0f, 0.0f),
				.rotation = Vec3(0.0f, 0.0f, -180.0f)
			});

		loadglTFFile(path);
		prepareUniformBuffers();
		setupDescriptors();
		preparePipelines();

		prepareImGui();

		buildCommandBuffers();

		
	};

	void Update(float dt){

		

		
	};

	void Clean(){
		 vkDestroyPipeline(example->device, pipelines.solid, nullptr);
		if (pipelines.wireframe != VK_NULL_HANDLE) {
			vkDestroyPipeline(example->device, pipelines.wireframe, nullptr);
		}

		vkDestroyPipelineLayout(example->device, pipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(example->device, descriptorSetLayouts.matrices, nullptr);
		vkDestroyDescriptorSetLayout(example->device, descriptorSetLayouts.textures, nullptr);

		shaderData.buffer.destroy();
		delete imGui;
	};
	
	void updateUniformBuffers(){
		auto& cam = gCoordinator.GetComponent<Camera>(example->camera);
		
		shaderData.values.model = updateViewMatrix();
		shaderData.values.projection =  glm::perspective(glm::radians(45.0f), float(example->width)/float(example->height), 0.1f, 256.0f);
		memcpy(shaderData.buffer.mapped, &shaderData.values, sizeof(shaderData.values));
	};
	void buildCommandBuffers(){
		VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

		VkClearValue clearValues[2];
		clearValues[0].color = example->defaultClearColor;
		clearValues[0].color = { { 0.25f, 0.25f, 0.25f, 1.0f } };;
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
		renderPassBeginInfo.renderPass = example->renderPass;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = example->width;
		renderPassBeginInfo.renderArea.extent.height = example->height;
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues = clearValues;

		imGui->newFrame(example, (example->frameCounter == 0));

		imGui->updateBuffers();

		const VkViewport viewport = vks::initializers::viewport((float)example->width, (float)example->height, 0.0f, 1.0f);
		const VkRect2D scissor = vks::initializers::rect2D(example->width, example->height, 0, 0);

		for (int32_t i = 0; i < example->drawCmdBuffers.size(); ++i)
		{
			renderPassBeginInfo.framebuffer = example->frameBuffers[i];
			VK_CHECK_RESULT(vkBeginCommandBuffer(example->drawCmdBuffers[i], &cmdBufInfo));
			vkCmdBeginRenderPass(example->drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdSetViewport(example->drawCmdBuffers[i], 0, 1, &viewport);
			vkCmdSetScissor(example->drawCmdBuffers[i], 0, 1, &scissor);
			// Bind scene matrices descriptor to set 0
			vkCmdBindDescriptorSets(example->drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
			vkCmdBindPipeline(example->drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, uiSettings.wireframe ? pipelines.wireframe : pipelines.solid);
			if (uiSettings.displayModels) {
				Transform transform = gCoordinator.GetComponent<Transform>(mesh);
				glTFModel.draw(example->drawCmdBuffers[i], pipelineLayout,transform);
			}
			
			example->drawUI(example->drawCmdBuffers[i]);
			// Render imGui
			imGui->drawFrame(example->drawCmdBuffers[i]);
			vkCmdEndRenderPass(example->drawCmdBuffers[i]);
			VK_CHECK_RESULT(vkEndCommandBuffer(example->drawCmdBuffers[i]));
		}
	};

private:
	std::bitset<8> mButtons;

	void InputListener(Event& event){
		mButtons = event.GetParam<std::bitset<8>>(Events::Window::Input::INPUT);
	};

    
    void prepareImGui(){
		imGui = new ImGUI(example);
		imGui->init((float)example->width, (float)example->height);
		imGui->initResources(example->renderPass, example->queue, example->getShadersPath());
	};
    void preparePipelines(){
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCI = vks::initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
		VkPipelineRasterizationStateCreateInfo rasterizationStateCI = vks::initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, 0);
		VkPipelineColorBlendAttachmentState blendAttachmentStateCI = vks::initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
		VkPipelineColorBlendStateCreateInfo colorBlendStateCI = vks::initializers::pipelineColorBlendStateCreateInfo(1, &blendAttachmentStateCI);
		VkPipelineDepthStencilStateCreateInfo depthStencilStateCI = vks::initializers::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
		VkPipelineViewportStateCreateInfo viewportStateCI = vks::initializers::pipelineViewportStateCreateInfo(1, 1, 0);
		VkPipelineMultisampleStateCreateInfo multisampleStateCI = vks::initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT, 0);
		const std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicStateCI = vks::initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables.data(), static_cast<uint32_t>(dynamicStateEnables.size()), 0);
		// Vertex input bindings and attributes
		const std::vector<VkVertexInputBindingDescription> vertexInputBindings = {
			vks::initializers::vertexInputBindingDescription(0, sizeof(VulkanglTFModel::Vertex), VK_VERTEX_INPUT_RATE_VERTEX),
		};
		const std::vector<VkVertexInputAttributeDescription> vertexInputAttributes = {
			vks::initializers::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanglTFModel::Vertex, pos)),	// Location 0: Position
			vks::initializers::vertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanglTFModel::Vertex, normal)),// Location 1: Normal
			vks::initializers::vertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanglTFModel::Vertex, uv)),	// Location 2: Texture coordinates
			vks::initializers::vertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanglTFModel::Vertex, color)),	// Location 3: Color
		};
		VkPipelineVertexInputStateCreateInfo vertexInputStateCI = vks::initializers::pipelineVertexInputStateCreateInfo();
		vertexInputStateCI.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindings.size());
		vertexInputStateCI.pVertexBindingDescriptions = vertexInputBindings.data();
		vertexInputStateCI.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
		vertexInputStateCI.pVertexAttributeDescriptions = vertexInputAttributes.data();

		const std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {
			example->loadShader(example->getShadersPath() + "gltfloading/mesh.vert.spv", VK_SHADER_STAGE_VERTEX_BIT),
			example->loadShader(example->getShadersPath() + "gltfloading/mesh.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT)
		};

		VkGraphicsPipelineCreateInfo pipelineCI = vks::initializers::pipelineCreateInfo(pipelineLayout, example->renderPass, 0);
		pipelineCI.pVertexInputState = &vertexInputStateCI;
		pipelineCI.pInputAssemblyState = &inputAssemblyStateCI;
		pipelineCI.pRasterizationState = &rasterizationStateCI;
		pipelineCI.pColorBlendState = &colorBlendStateCI;
		pipelineCI.pMultisampleState = &multisampleStateCI;
		pipelineCI.pViewportState = &viewportStateCI;
		pipelineCI.pDepthStencilState = &depthStencilStateCI;
		pipelineCI.pDynamicState = &dynamicStateCI;
		pipelineCI.stageCount = static_cast<uint32_t>(shaderStages.size());
		pipelineCI.pStages = shaderStages.data();

		// Solid rendering pipeline
		VK_CHECK_RESULT(vkCreateGraphicsPipelines(example->device, example->pipelineCache, 1, &pipelineCI, nullptr, &pipelines.solid));

		// Wire frame rendering pipeline
		if (example->deviceFeatures.fillModeNonSolid) {
			rasterizationStateCI.polygonMode = VK_POLYGON_MODE_LINE;
			rasterizationStateCI.lineWidth = 1.0f;
			VK_CHECK_RESULT(vkCreateGraphicsPipelines(example->device, example->pipelineCache, 1, &pipelineCI, nullptr, &pipelines.wireframe));
		}
	};
    void setupDescriptors(){
		/*
			This sample uses separate descriptor sets (and layouts) for the matrices and materials (textures)
		*/

		std::vector<VkDescriptorPoolSize> poolSizes = {
			vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
			// One combined image sampler per model image/texture
			vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(glTFModel.images.size())),
		};
		// One set for matrices and one per model image/texture
		const uint32_t maxSetCount = static_cast<uint32_t>(glTFModel.images.size()) + 1;
		VkDescriptorPoolCreateInfo descriptorPoolInfo = vks::initializers::descriptorPoolCreateInfo(poolSizes, maxSetCount);
		VK_CHECK_RESULT(vkCreateDescriptorPool(example->device, &descriptorPoolInfo, nullptr, &example->descriptorPool));

		// Descriptor set layout for passing matrices
		VkDescriptorSetLayoutBinding setLayoutBinding = vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = vks::initializers::descriptorSetLayoutCreateInfo(&setLayoutBinding, 1);
		VK_CHECK_RESULT(vkCreateDescriptorSetLayout(example->device, &descriptorSetLayoutCI, nullptr, &descriptorSetLayouts.matrices));
		// Descriptor set layout for passing material textures
		setLayoutBinding = vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0);
		VK_CHECK_RESULT(vkCreateDescriptorSetLayout(example->device, &descriptorSetLayoutCI, nullptr, &descriptorSetLayouts.textures));
		// Pipeline layout using both descriptor sets (set 0 = matrices, set 1 = material)
		std::array<VkDescriptorSetLayout, 2> setLayouts = { descriptorSetLayouts.matrices, descriptorSetLayouts.textures };
		VkPipelineLayoutCreateInfo pipelineLayoutCI= vks::initializers::pipelineLayoutCreateInfo(setLayouts.data(), static_cast<uint32_t>(setLayouts.size()));
		// We will use push constants to push the local matrices of a primitive to the vertex shader
		VkPushConstantRange pushConstantRange = vks::initializers::pushConstantRange(VK_SHADER_STAGE_VERTEX_BIT, sizeof(glm::mat4), 0);
		// Push constant ranges are part of the pipeline layout
		pipelineLayoutCI.pushConstantRangeCount = 1;
		pipelineLayoutCI.pPushConstantRanges = &pushConstantRange;
		VK_CHECK_RESULT(vkCreatePipelineLayout(example->device, &pipelineLayoutCI, nullptr, &pipelineLayout));

		// Descriptor set for scene matrices
		VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(example->descriptorPool, &descriptorSetLayouts.matrices, 1);
		VK_CHECK_RESULT(vkAllocateDescriptorSets(example->device, &allocInfo, &descriptorSet));
		VkWriteDescriptorSet writeDescriptorSet = vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &shaderData.buffer.descriptor);
		vkUpdateDescriptorSets(example->device, 1, &writeDescriptorSet, 0, nullptr);
		// Descriptor sets for materials
		for (auto& image : glTFModel.images) {
			const VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(example->descriptorPool, &descriptorSetLayouts.textures, 1);
			VK_CHECK_RESULT(vkAllocateDescriptorSets(example->device, &allocInfo, &image.descriptorSet));
			VkWriteDescriptorSet writeDescriptorSet = vks::initializers::writeDescriptorSet(image.descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &image.texture.descriptor);
			vkUpdateDescriptorSets(example->device, 1, &writeDescriptorSet, 0, nullptr);
		}
	};
    void prepareUniformBuffers(){
		// Vertex shader uniform buffer block
		VK_CHECK_RESULT(example->vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&shaderData.buffer,
			sizeof(shaderData.values)));

		// Map persistent
		VK_CHECK_RESULT(shaderData.buffer.map());

		updateUniformBuffers();
	};
	
	glm::mat4 updateViewMatrix(){
		auto& transform = gCoordinator.GetComponent<Transform>(example->camera);
		glm::mat4 view;
		glm::mat4 rotM = glm::mat4(1.0f);
		glm::mat4 transM;

		rotM = glm::rotate(rotM, glm::radians(transform.rotation.x ), glm::vec3(1.0f, 0.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 translation = glm::vec3(transform.position.x,transform.position.y,transform.position.z);
	
	
		
		transM = glm::translate(glm::mat4(1.0f), translation);

		//if (type == CameraType::firstperson)
		//{
			view = rotM * transM;
		//}
		//else
		//{
			//view = transM * rotM;
		//}

		return view;

	
	};


	void loadglTFFile(std::string filename){
		tinygltf::Model glTFInput;
		tinygltf::TinyGLTF gltfContext;
		std::string error, warning;

		this->device = example->device;

#if defined(__ANDROID__)
		// On Android all assets are packed with the apk in a compressed form, so we need to open them using the asset manager
		// We let tinygltf handle this, by passing the asset manager of our app
		tinygltf::asset_manager = androidApp->activity->assetManager;
#endif
		bool fileLoaded = gltfContext.LoadASCIIFromFile(&glTFInput, &error, &warning, filename);

		// Pass some Vulkan resources required for setup and rendering to the glTF model loading class
		glTFModel.vulkanDevice = example->vulkanDevice;
		glTFModel.copyQueue = example->queue;

		std::vector<uint32_t> indexBuffer;
		std::vector<VulkanglTFModel::Vertex> vertexBuffer;

		if (fileLoaded) {
			glTFModel.loadImages(glTFInput);
			glTFModel.loadMaterials(glTFInput);
			glTFModel.loadTextures(glTFInput);
			const tinygltf::Scene& scene = glTFInput.scenes[0];
			for (size_t i = 0; i < scene.nodes.size(); i++) {
				const tinygltf::Node node = glTFInput.nodes[scene.nodes[i]];
				glTFModel.loadNode(node, glTFInput, nullptr, indexBuffer, vertexBuffer);
			}
		}
		else {
			vks::tools::exitFatal("Could not open the glTF file.\n\nThe file is part of the additional asset pack.\n\nRun \"download_assets.py\" in the repository root to download the latest version.", -1);
			return;
		}

		// Create and upload vertex and index buffer
		// We will be using one single vertex buffer and one single index buffer for the whole glTF scene
		// Primitives (of the glTF model) will then index into these using index offsets

		size_t vertexBufferSize = vertexBuffer.size() * sizeof(VulkanglTFModel::Vertex);
		size_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
		glTFModel.indices.count = static_cast<uint32_t>(indexBuffer.size());

		struct StagingBuffer {
			VkBuffer buffer;
			VkDeviceMemory memory;
		} vertexStaging, indexStaging;

		// Create host visible staging buffers (source)
		VK_CHECK_RESULT(example->vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBufferSize,
			&vertexStaging.buffer,
			&vertexStaging.memory,
			vertexBuffer.data()));
		// Index data
		VK_CHECK_RESULT(example->vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			indexBufferSize,
			&indexStaging.buffer,
			&indexStaging.memory,
			indexBuffer.data()));

		// Create device local buffers (target)
		VK_CHECK_RESULT(example->vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBufferSize,
			&glTFModel.vertices.buffer,
			&glTFModel.vertices.memory));
		VK_CHECK_RESULT(example->vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBufferSize,
			&glTFModel.indices.buffer,
			&glTFModel.indices.memory));

		// Copy data from staging buffers (host) do device local buffer (gpu)
		VkCommandBuffer copyCmd = example->vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
		VkBufferCopy copyRegion = {};

		copyRegion.size = vertexBufferSize;
		vkCmdCopyBuffer(
			copyCmd,
			vertexStaging.buffer,
			glTFModel.vertices.buffer,
			1,
			&copyRegion);

		copyRegion.size = indexBufferSize;
		vkCmdCopyBuffer(
			copyCmd,
			indexStaging.buffer,
			glTFModel.indices.buffer,
			1,
			&copyRegion);

		example->vulkanDevice->flushCommandBuffer(copyCmd, example->queue, true);

		// Free staging resources
		vkDestroyBuffer(example->device, vertexStaging.buffer, nullptr);
		vkFreeMemory(example->device, vertexStaging.memory, nullptr);
		vkDestroyBuffer(example->device, indexStaging.buffer, nullptr);
		vkFreeMemory(example->device, indexStaging.memory, nullptr);
	};

   

    

    
};

#endif
