#pragma once
#ifndef MESH_SYSTEM_H
#define MESH_SYSTEM_H

#include "../System.hpp"
#include "../../vulkan_example_base/vulkan_example_base.h"
#include "../../gui/gui.hpp"
#include "../../gui/guizmo.hpp"
#include "../components/Transform.hpp"
#include "../components/Camera.hpp"
#include "../components/Renderable.hpp"
#include "../Coordinator.hpp"

#include "../../../third_party/imgui/imgui.h"
#include "../../../third_party/imGuizmo/ImGuizmo.h"
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

		const VkViewport viewport = vks::initializers::viewport(imGui->vMin.x, imGui->vMin.y,(float)imGui->editor_size.x, (float)imGui->editor_size.y,0.0f,1.0f);
		const VkRect2D scissor = vks::initializers::rect2D(imGui->editor_size.x-10, imGui->editor_size.y-10, imGui->offset.x, imGui->offset.y);
		

		for (int32_t i = 0; i < example->drawCmdBuffers.size(); ++i)
		{
			renderPassBeginInfo.framebuffer = example->frameBuffers[i];
			VK_CHECK_RESULT(vkBeginCommandBuffer(example->drawCmdBuffers[i], &cmdBufInfo));
			vkCmdBeginRenderPass(example->drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			
			example->drawUI(example->drawCmdBuffers[i]);

			// Render imGui
			ImGui::NewFrame();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoBackground;


			imGui->newFrame(example, (example->frameCounter == 0), mEntities);
			ImGuiViewport* vport = ImGui::GetMainViewport();

		ImGui::SetNextWindowViewport(vport->ID);
		ImGui::SetNextWindowBgAlpha(0.0f);

			ImGui::Begin("Editor" , nullptr,
				ImGuiWindowFlags_NoBringToFrontOnFocus);

			if ((example->mousePos.x >= imGui->vMin.x && example->mousePos.x <= imGui->vMax.x - 10) && (example->mousePos.y >= imGui->vMin.y && example->mousePos.y <= imGui->vMax.y - 10)) {
				//io.WantCaptureMouse = false;
				io.ConfigWindowsMoveFromTitleBarOnly = true;
			}

			{
				imGui->editor_size = ImGui::GetWindowSize();
				imGui->vMin = ImGui::GetWindowContentRegionMin();
				imGui->vMax = ImGui::GetWindowContentRegionMax();

				imGui->vMin.x += ImGui::GetWindowPos().x;
				imGui->vMin.y += ImGui::GetWindowPos().y;
				imGui->vMax.x += ImGui::GetWindowPos().x;
				imGui->vMax.y += ImGui::GetWindowPos().y;

				imGui->offset.x = 0;
				imGui->offset.y = 0;

				imGui->offset.x += ImGui::GetWindowPos().x;
				imGui->offset.y += ImGui::GetWindowPos().y;



			}
		
			ImGuizmo::SetDrawlist();


			for (auto& entity : mEntities)
			{

				auto& renderable = gCoordinator.GetComponent<Renderable>(entity);
				auto& transform = gCoordinator.GetComponent<Transform>(entity);
				auto& meshTransform = gCoordinator.GetComponent<Transform>(entity);
				auto& cam = gCoordinator.GetComponent<Camera>(example->camera);

				vkCmdSetViewport(example->drawCmdBuffers[i], 0, 1, &viewport);
				vkCmdSetScissor(example->drawCmdBuffers[i], 0, 1, &scissor);
				vkCmdBindPipeline(example->drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, uiSettings.wireframe ? renderable.pipelines.wireframe : renderable.pipelines.solid);
				vkCmdBindDescriptorSets(example->drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderable.pipelineLayout, 0, 1, &renderable.descriptorSet, 0, nullptr);

				printf("\n.... CALLED DRAW ON GLTF MODEL .... \n");
				if (renderable.path.length() > 10) {
					Renderable::draw(example, renderable.model, renderable.pipelineLayout, transform, i);

				}

				for (auto& node : renderable.model.nodes)
				{



					glm::mat4 rotM = glm::mat4(1.0f);
					glm::mat4 transM;
					glm::mat4 scaleM;

					rotM = glm::rotate(rotM, glm::radians(meshTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
					rotM = glm::rotate(rotM, glm::radians(meshTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
					rotM = glm::rotate(rotM, glm::radians(meshTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

					glm::vec3 translation = glm::vec3(meshTransform.position.x, meshTransform.position.y, meshTransform.position.z);
					glm::vec3 rotation = glm::vec3(meshTransform.rotation.x, meshTransform.rotation.y, meshTransform.rotation.z);
					glm::vec3 scale = glm::vec3(meshTransform.scale.x, meshTransform.scale.y, meshTransform.scale.z);



					transM = glm::translate(glm::mat4(1.0f), translation);

					scaleM = glm::scale(glm::mat4(1.0f), scale);

					//if (type == CameraType::firstperson)
					//{
					glm::mat4 nodeMatrix = scaleM * rotM * transM * node.matrix;
					ImGuiIO& io = ImGui::GetIO();
					ImGuizmo::SetRect(imGui->vMin.x, imGui->vMin.y, imGui->editor_size.x, imGui->editor_size.y);

					ImGuizmo::Manipulate(glm::value_ptr(renderable.updateViewMatrix(example)), glm::value_ptr(cam.projection), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(nodeMatrix), NULL, NULL);
					ImGuizmo::DecomposeMatrixToComponents(
						glm::value_ptr(nodeMatrix),
						glm::value_ptr(translation),
						glm::value_ptr(rotation),
						glm::value_ptr(scale));

					meshTransform.position.x = translation.x;
					meshTransform.position.y = translation.y;
					meshTransform.position.z = translation.z;


				
				}

			}


	
			ImGui::End();

			
			
			ImGui::Render();



		
			imGui->updateBuffers();
			imGui->drawFrame(example->drawCmdBuffers[i]);


			
			
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
