/*
* Vulkan Example - glTF scene loading and rendering
*
* Copyright (C) 2020 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

/*
 * Shows how to load and display a simple scene from a glTF file
 * Note that this isn't a complete glTF loader and only basic functions are shown here
 * This means no complex materials, no animations, no skins, etc.
 * For details on how glTF 2.0 works, see the official spec at https://github.com/KhronosGroup/glTF/tree/master/specification/2.0
 *
 * Other samples will load models using a dedicated model loader with more features (see base/VulkanglTFModel.hpp)
 *
 * If you are looking for a complete glTF implementation, check out https://github.com/SaschaWillems/Vulkan-glTF-PBR/
 */



#include "ecs/components/Camera.hpp"
#include "ecs/components/Transform.hpp"

#include "ecs/Coordinator.hpp"

#include "ecs/systems/CameraControlSystem.hpp"
#include "vulkan_example_base/vulkan_example_base.h"


#define ENABLE_VALIDATION false


#include "ecs/systems/MeshSystem.hpp"



Coordinator gCoordinator;

// ----------------------------------------------------------------------------
// VulkanExample
// ----------------------------------------------------------------------------

class VulkanExample : public VulkanExampleBase
{
public:
	
	//end nodes
	std::shared_ptr<CameraControlSystem> cameraControlSystem;
	std::shared_ptr<MeshSystem> meshSystem;





	VulkanExample() : VulkanExampleBase(ENABLE_VALIDATION)
	{
		title = "Paradise Engine";
		gCoordinator.Init();

		gCoordinator.RegisterComponent<Camera>();
		gCoordinator.RegisterComponent<Transform>();

		cameraControlSystem = gCoordinator.RegisterSystem<CameraControlSystem>();
		{
			Signature signature;
			signature.set(gCoordinator.GetComponentType<Camera>());
			signature.set(gCoordinator.GetComponentType<Transform>());
			gCoordinator.SetSystemSignature<CameraControlSystem>(signature);
		}

		cameraControlSystem->Init();

		meshSystem = gCoordinator.RegisterSystem<MeshSystem>();
		{
			Signature signature;
			signature.set(gCoordinator.GetComponentType<Camera>());
			signature.set(gCoordinator.GetComponentType<Transform>());
			gCoordinator.SetSystemSignature<MeshSystem>(signature);
		}

		

	}

	~VulkanExample()
	{
	
		// Clean up used Vulkan resources
		// Note : Inherited destructor cleans up resources stored in base class
		
		meshSystem->Clean();

	
	}





	void draw()
	{
		VulkanExampleBase::prepareFrame();
		meshSystem->buildCommandBuffers();
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
		VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
		VulkanExampleBase::submitFrame();
	}

	

	void prepare()
	{
		
		VulkanExampleBase::prepare();

		//TODO(eduardo) init all meshSystems 
		meshSystem->Init(this,getAssetPath() + "models/Warrior.gltf");

		prepared = true;
	}

	virtual void render()
	{
		if (!prepared)
			return;

		// Update imGui
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2((float)width, (float)height);
		io.DeltaTime = frameTimer;

		io.MousePos = ImVec2(mousePos.x, mousePos.y);
		io.MouseDown[0] = mouseBtns.left;
		io.MouseDown[1] = mouseBtns.right;

		draw();

		meshSystem->updateUniformBuffers();
	}

	virtual void viewChanged()
	{
		meshSystem->updateUniformBuffers();
	}

	

	void Update(float dt){
		cameraControlSystem->Update(dt);
		meshSystem->Update(dt);
	}

};

VULKAN_EXAMPLE_MAIN()