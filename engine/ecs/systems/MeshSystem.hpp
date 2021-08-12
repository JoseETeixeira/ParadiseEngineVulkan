#pragma once

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

	void Init(VulkanExampleBase *ex);

	void Update(float dt);

	void Clean();
	
	void updateUniformBuffers();
	void buildCommandBuffers();

private:
	std::bitset<8> mButtons;

	void InputListener(Event& event);

    
    void prepareImGui();
    void preparePipelines();
    void setupDescriptors();
    void prepareUniformBuffers();
	
	glm::mat4 updateViewMatrix();
	void loadAssets();
	void loadglTFFile(std::string filename);

    Entity mesh;

    

    
};
