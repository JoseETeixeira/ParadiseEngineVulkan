#pragma once
#ifndef MESH_SYSTEM_H
#define MESH_SYSTEM_H

#include "../System.hpp"
#include "../../vulkan_example_base/vulkan_example_base.h"
#include "../../gui/gui.hpp"
#include "../components/Transform.hpp"
#include "../components/Camera.h"
#include "../components/Renderable.hpp"
#include "../Coordinator.hpp"

#include "../../../third_party/imgui/imgui.h"
#include "../../../third_party/imGuizmo/ImGuizmo.h"
#include "../../../third_party/imgui/backends/imgui_impl_vulkan.h"
#include "../math/Vec3.hpp"
#include "../math/Vec2.hpp"
#include "../../vulkan_gltf_model/vulkan_gltf_model.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <algorithm>

class Event;

extern Coordinator gCoordinator;

class MeshSystem : public System
{
public:

    VulkanExampleBase *example;
	ImGUI *imGui = nullptr;

	bool useWindow = false;
	int gizmoCount = 1;
	float camDistance = 8.f;
	ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	

	const float identityMatrix[16] =
{ 1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f };


	void Init(VulkanExampleBase *ex){
		printf(".... INITIALIZING MESH SYSTEM .... \n");
		example = ex;
		for (auto& entity : mEntities)
		{
			auto& renderable = gCoordinator.GetComponent<Renderable>(entity);
			auto& meshTransform = gCoordinator.GetComponent<Transform>(entity);
			Renderable::prepareMesh(renderable.path,ex,renderable.model,renderable.descriptorSetLayouts,renderable.shaderData,renderable.pipelines,renderable.pipelineLayout,renderable.descriptorSet,renderable.device,meshTransform );


		}

		prepareImGui();
		printf(".... FINISHED INITIALIZING MESH SYSTEM .... \n");

		
		
	};

	void prepareImGui(){
		imGui = new ImGUI(example);
		imGui->init((float)example->width, (float)example->height);
		imGui->initResources(example->renderPass, example->queue, example->getShadersPath());
	};




void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition,Transform &meshTransform,glm::vec3 &translation,glm::vec3 &rotation,glm::vec3 &scale)
{
   static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
   static bool useSnap = false;
   static float snap[3] = { 1.f, 1.f, 1.f };
   static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
   static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
   static bool boundSizing = false;
   static bool boundSizingSnap = false;
 
  

   if (editTransformDecomposition)
   {
      ImGuiIO& io = ImGui::GetIO();

   ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
   ImGuizmo::DrawCubes(cameraView, cameraProjection, matrix, gizmoCount);
   ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
	
	ImGuizmo::DecomposeMatrixToComponents(
					matrix,
					glm::value_ptr(translation),
					glm::value_ptr(rotation),
					glm::value_ptr(scale));

		

					
	

   }

  

}



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

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoBackground;
			ImGui::NewFrame();
			


			imGui->newFrame(example, (example->frameCounter == 0), mEntities);
			ImGuiViewport* vport = ImGui::GetMainViewport();

		ImGui::SetNextWindowViewport(vport->ID);
		ImGui::SetNextWindowBgAlpha(0.0f);

			ImGui::Begin("Editor" , nullptr,
				ImGuiWindowFlags_NoBringToFrontOnFocus);

			if ((example->mousePos.x >= imGui->vMin.x && example->mousePos.x <= imGui->vMax.x - 10) && (example->mousePos.y >= imGui->vMin.y && example->mousePos.y <= imGui->vMax.y - 10)) {
				io.WantCaptureMouse = false;
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
		
			

			int matId = 0;
			int lastUsing = 0;
			float *matrix;
			glm::vec3 translation;
			glm::vec3 rotation;
			glm::vec3 scale;
			Transform mtransform;

			for (auto& entity : mEntities)
			{

				ImGui::PushID(matId);

				auto& renderable = gCoordinator.GetComponent<Renderable>(entity);
				auto& meshTransform = gCoordinator.GetComponent<Transform>(entity);
				auto& cam = gCoordinator.GetComponent<Camera>(example->camera);
				auto& cameraTransform = gCoordinator.GetComponent<Transform>(example->camera);

				
				translation = glm::vec3(0.0f , 0.0f , 0.0f);
				rotation = glm::vec3(0.0f, 0.0f ,0.0f );
				scale = glm::vec3(1.0f, 1.0f, 1.0f);


				glm::mat4 rotM = glm::mat4(1.0f);
				glm::mat4 transM;
				glm::mat4 scaleM;

				

				rotM = glm::rotate(rotM, glm::radians(meshTransform.rotation.x ), glm::vec3(1.0f, 0.0f, 0.0f));
				rotM = glm::rotate(rotM, glm::radians(meshTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				rotM = glm::rotate(rotM, glm::radians(meshTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				
				
				glm::vec3 tempTrans = glm::vec3(meshTransform.position.x,meshTransform.position.y *-1.0f,meshTransform.position.z);


				transM = glm::translate(glm::mat4(1.0f), tempTrans);

				scaleM = glm::scale(glm::mat4(1.0f), scale);

				//if (type == CameraType::firstperson)
				//{
				glm::mat4 nodeMatrix =  scaleM* rotM * transM ;

				ImGuizmo::SetID(matId);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetRect(imGui->vMin.x, imGui->vMin.y, imGui->editor_size.x, imGui->editor_size.y);

					

				//view matrix is the inverse of the camera matrix

				//TODO: pass view matrix and projection to the model then render from the result

				matrix = (float*)glm::value_ptr(nodeMatrix);

				EditTransform(cam.getViewMatRef(), cam.getProjMatRef(), matrix, lastUsing == matId,meshTransform,translation,rotation,scale);
				
				if (ImGuizmo::IsUsing())
				{
				
					lastUsing = matId;
					io.WantCaptureMouse = true;



					meshTransform.position.x += translation.x/1000;
					meshTransform.position.y -= translation.y/1000;
					meshTransform.position.z += translation.z/1000;
									
				}


			

				

				
			
				matId += 1;	

				ImGui::PopID();

				

			}
	
			ImGui::End();

			
			
			ImGui::Render();



		
			imGui->updateBuffers();
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
			auto& meshTransform = gCoordinator.GetComponent<Transform>(entity);
			if(renderable.path.length() > 10 ){
				Renderable::updateUniformBuffers(example,renderable.shaderData,meshTransform);
			
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
