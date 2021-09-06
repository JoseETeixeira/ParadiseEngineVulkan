#pragma once
#ifndef MESH_SYSTEM_H
#define MESH_SYSTEM_H

#include "../System.hpp"
#include "../../vulkan_example_base/vulkan_example_base.h"
#include "../../gui/gui.hpp"
#include "../components/Transform.hpp"
#include "../components/Camera.hpp"
#include "../components/Renderable.hpp"
#include "../Coordinator.hpp"
#include "../../../third_party/imgui/backends/imgui_impl_vulkan.h"
#include "../math/Vec3.hpp"
#include "../math/Vec2.hpp"
#include "../../vulkan_gltf_model/vulkan_gltf_model.hpp"

#include <algorithm>

class Event;

extern Coordinator gCoordinator;

class MeshSystem : public System
{
public:
   VulkanExampleBase *example;
	ImGUI *imGui = nullptr;
	void Init(VulkanExampleBase *ex){
		printf(".... INITIALIZING MESH SYSTEM .... \n");
		example = ex;
		for (auto& entity : mEntities)
		{
			auto& renderable = gCoordinator.GetComponent<Renderable>(entity);
			Renderable::prepareMesh(renderable.path,ex,renderable.model,renderable.descriptorSetLayouts,renderable.shaderData,renderable.pipelines,renderable.pipelineLayout,renderable.descriptorSet,renderable.device );


		}

		prepareImGui();
		printf(".... FINISHED INITIALIZING MESH SYSTEM .... \n");

		
		
	};

	void prepareImGui(){
		imGui = new ImGUI(example);
		imGui->init((float)example->width, (float)example->height);
		imGui->initResources(example->renderPass, example->queue, example->getShadersPath());
	};

	void Draw(){
		printf(".... INITIALIZING MESH SYSTEM DRAW .... \n");
		printf("\n.... INIT BUILDING COMMAND BUFFERS .... \n");
		
		VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

		VkClearValue clearValues[5];
		clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
		clearValues[1].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
		clearValues[2].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
		clearValues[3].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
		clearValues[4].depthStencil = { 1.0f, 0 };


		VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
		renderPassBeginInfo.renderPass = example->renderPass;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = example->width;
		renderPassBeginInfo.renderArea.extent.height = example->height;
		renderPassBeginInfo.clearValueCount = 5;
		renderPassBeginInfo.pClearValues = clearValues;


		imGui->newFrame(example, (example->frameCounter == 0));

		imGui->updateBuffers();


		const VkViewport viewport = vks::initializers::viewport(imGui->vMin.x, imGui->vMin.y,(float)imGui->editor_size.x, (float)imGui->editor_size.y,0.0f,1.0f);
		const VkRect2D scissor = vks::initializers::rect2D(imGui->editor_size.x-10, imGui->editor_size.y-10, imGui->offset.x, imGui->offset.y);
		

		for (int32_t i = 0; i < example->drawCmdBuffers.size(); ++i)
		{
			renderPassBeginInfo.framebuffer = example->frameBuffers[i];
			VK_CHECK_RESULT(vkBeginCommandBuffer(example->drawCmdBuffers[i], &cmdBufInfo));
			vkCmdBeginRenderPass(example->drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			
			
			example->drawUI(example->drawCmdBuffers[i]);
			// Render imGui
			imGui->drawFrame(example->drawCmdBuffers[i]);

			vkCmdNextSubpass(example->drawCmdBuffers[i], VK_SUBPASS_CONTENTS_INLINE);

				for (auto& entity : mEntities)
				{
					
					auto& renderable = gCoordinator.GetComponent<Renderable>(entity);
					auto& transform = gCoordinator.GetComponent<Transform>(entity);

					vkCmdSetViewport(example->drawCmdBuffers[i], 0, 1, &viewport);
					vkCmdSetScissor(example->drawCmdBuffers[i], 0, 1, &scissor);
					vkCmdBindPipeline(example->drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, uiSettings.wireframe ? renderable.pipelines.wireframe : renderable.pipelines.solid);
					vkCmdBindDescriptorSets(example->drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderable.pipelineLayout, 0, 1, &renderable.descriptorSet, 0, nullptr);
					
					printf("\n.... CALLED DRAW ON GLTF MODEL .... \n");
					if(renderable.path.length() > 10 ){
						Renderable::draw(example,renderable.model,renderable.pipelineLayout,transform,i);
					
					}
					

				}
			vkCmdEndRenderPass(example->drawCmdBuffers[i]);
			VK_CHECK_RESULT(vkEndCommandBuffer(example->drawCmdBuffers[i]));
		}
		printf(".... FINISHED  MESH SYSTEM DRAW .... \n");
		
	};



	void Update(float dt){

		for (auto& entity : mEntities)
		{
			auto& renderable = gCoordinator.GetComponent<Renderable>(entity);
			auto& transform = gCoordinator.GetComponent<Transform>(entity);
			if(renderable.path.length() > 10 ){
				Renderable::updateUniformBuffers(example,renderable.shaderData);
			
			}
			

		}

		
	};

	~MeshSystem(){
		
	}

	

	
	
	

private:
	std::bitset<8> mButtons;

	void InputListener(Event& event){
		mButtons = event.GetParam<std::bitset<8>>(Events::Window::Input::INPUT);
	};



    
};

#endif
