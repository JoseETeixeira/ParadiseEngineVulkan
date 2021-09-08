#pragma once

#include "../math/Vec3.hpp"
#include "../math/Vec2.hpp"
#include "../../vulkan_example_base/vulkan_example_base.h"
#include "Transform.hpp"
#include "Camera.hpp"
#include "../../vulkan_gltf_model/vulkan_gltf_model.hpp"

#include <algorithm>


extern Coordinator gCoordinator;

struct Renderable
{
	std::string path;
	VulkanglTFModel model;
	bool enabled = false;
	

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

	static void prepareMesh(std::string path,VulkanExampleBase *example,VulkanglTFModel &model, DescriptorSetLayouts &descriptorSetLayouts,ShaderData  &shaderData , Pipelines &pipelines, VkPipelineLayout &pipelineLayout, VkDescriptorSet &descriptorSet,VkDevice &device ){
		printf("\n.... PREPARING MESH .... \n");
		prepareUniformBuffers(example,shaderData);
		setupDescriptors(example,model,descriptorSetLayouts,pipelineLayout,shaderData,descriptorSet);
		preparePipelines(example,pipelines,pipelineLayout);
		printf("\n.... FINISHED PREPARING MESH .... \n");

		
	}

	static void draw(VulkanExampleBase *example,VulkanglTFModel &glTFModel, VkPipelineLayout &pipelineLayout,Transform transform,int32_t i){
		printf("\n.... INIT DRAWING .... \n");
		
		buildCommandBuffers(example,glTFModel,pipelineLayout,transform,i);
	
		printf("\n.... FINISHED DRAWING .... \n");
	}

	static void buildCommandBuffers(VulkanExampleBase *example,VulkanglTFModel &glTFModel, VkPipelineLayout &pipelineLayout,Transform transform,int32_t i){
		
				
			glTFModel.draw(example->drawCmdBuffers[i], pipelineLayout,transform);

			printf("\n.... FINISHED DRAW ON GLTF MODEL .... \n");

		
		printf("\n.... FINISHED BUILDING COMMAND BUFFERS .... \n");
	};

	static void preparePipelines(VulkanExampleBase *example, Pipelines &pipelines, VkPipelineLayout &pipelineLayout){
		printf("\n.... INIT PREPARE PIPELINES .... \n");

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

		printf("\n.... FINISH PREPARE PIPELINES .... \n");
	};

	static void setupDescriptors(VulkanExampleBase *example,VulkanglTFModel &glTFModel, DescriptorSetLayouts &descriptorSetLayouts,VkPipelineLayout &pipelineLayout,ShaderData  &shaderData, VkDescriptorSet &descriptorSet){
		/*
			This sample uses separate descriptor sets (and layouts) for the matrices and materials (textures)
		*/
		printf("\n.... INIT SETUP DESCRIPTORS .... \n");

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

		printf("\n.... FINISH SETUP PIPELINES .... \n");
	};

	static glm::mat4 updateViewMatrix(VulkanExampleBase *example){
		auto& transform = gCoordinator.GetComponent<Transform>(example->camera);
		glm::mat4 view;
		glm::mat4 rotM = glm::mat4(1.0f);
		glm::mat4 transM;
		glm::mat4 scaleM;

		rotM = glm::rotate(rotM, glm::radians(transform.rotation.x  ), glm::vec3(1.0f, 0.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(transform.rotation.y ), glm::vec3(0.0f, 1.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(transform.rotation.z + 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 translation = glm::vec3(transform.position.x, transform.position.y, transform.position.z);

		glm::vec3 rotation = glm::vec3(transform.rotation.x, transform.rotation.y, transform.rotation.z);
		glm::vec3 scale = glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z);



		transM = glm::translate(glm::mat4(1.0f), translation);

		scaleM = glm::scale(glm::mat4(1.0f), scale);

		//if (type == CameraType::firstperson)
		//{
		view = scaleM * transM * rotM ;


		return view;

	
	};

	static void prepareUniformBuffers(VulkanExampleBase *example,ShaderData  &shaderData){
		printf("\n.... INIT PREPARE UNIFORM BUFFERS .... \n");
		// Vertex shader uniform buffer block
		VK_CHECK_RESULT(example->vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&shaderData.buffer,
			sizeof(shaderData.values)));

		// Map persistent
		VK_CHECK_RESULT(shaderData.buffer.map());

		updateUniformBuffers(example,shaderData);

		printf("\n.... FINISH PREPARE UNIFORM BUFFERS .... \n");
	};

	static void updateUniformBuffers(VulkanExampleBase *example,ShaderData  &shaderData){

		printf("\n.... INIT UPDATE UNIFORM BUFFERS .... \n");
		auto& cam = gCoordinator.GetComponent<Camera>(example->camera);
		
		shaderData.values.model = updateViewMatrix(example);
		shaderData.values.projection =  glm::perspective(glm::radians(45.0f), float(example->width)/float(example->height), 0.1f, 256.0f);
		memcpy(shaderData.buffer.mapped, &shaderData.values, sizeof(shaderData.values));

		printf("\n.... FINISH PREPARE UNIFORM BUFFERS .... \n");
	};

	static void Clean(VulkanExampleBase *example, Pipelines &pipelines, DescriptorSetLayouts &descriptorSetLayouts, VkPipelineLayout &pipelineLayout, ShaderData  &shaderData){
		
		vkDestroyPipeline(example->device, pipelines.solid, nullptr);
		if (pipelines.wireframe != VK_NULL_HANDLE) {
			vkDestroyPipeline(example->device, pipelines.wireframe, nullptr);
		}

		vkDestroyPipelineLayout(example->device, pipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(example->device, descriptorSetLayouts.matrices, nullptr);
		vkDestroyDescriptorSetLayout(example->device, descriptorSetLayouts.textures, nullptr);

		shaderData.buffer.destroy();


	}

	static VulkanglTFModel loadglTFFile(std::string filename,VulkanExampleBase *example){

		printf("\n.... INIT LOADING GLTF FILE .... \n");
		VulkanglTFModel glTFModel;
		tinygltf::Model glTFInput;
		tinygltf::TinyGLTF gltfContext;
		std::string error, warning;

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
			return glTFModel; 
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


		printf("\n.... FINISHED LOADING GLTF FILE .... \n");
	
		return glTFModel; 
	};

	

};
