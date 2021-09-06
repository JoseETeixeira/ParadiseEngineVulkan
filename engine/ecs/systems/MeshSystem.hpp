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

	void Init(VulkanExampleBase *ex){
		printf(".... INITIALIZING MESH SYSTEM .... \n");
		example = ex;
		for (auto& entity : mEntities)
		{
			auto& renderable = gCoordinator.GetComponent<Renderable>(entity);
			Renderable::prepareMesh(renderable.path,ex,renderable.model,renderable.descriptorSetLayouts,renderable.shaderData,renderable.pipelines,renderable.pipelineLayout,renderable.descriptorSet,renderable.device );


		}
		printf(".... FINISHED INITIALIZING MESH SYSTEM .... \n");
		
	};

	void Draw(){
		printf(".... INITIALIZING MESH SYSTEM DRAW .... \n");

		for (auto& entity : mEntities)
		{
			auto& renderable = gCoordinator.GetComponent<Renderable>(entity);
			auto& transform = gCoordinator.GetComponent<Transform>(entity);
			if(renderable.path.length() > 10 ){
				Renderable::draw(example,renderable.model,transform,Vec2(0,0),Vec2(example->width,example->height),Vec2(0,0),renderable.pipelines,renderable.pipelineLayout,renderable.descriptorSet);
			
			}
			

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
