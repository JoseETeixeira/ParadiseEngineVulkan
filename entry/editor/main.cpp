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



#include "../../engine/ecs/components/Camera.hpp"
#include "../../engine/ecs/components/Transform.hpp"

#include "../../engine/ecs/Coordinator.hpp"

#include "../../engine/ecs/systems/CameraControlSystem.hpp"
#include "../../engine/vulkan_example_base/vulkan_example_base.h"


#define ENABLE_VALIDATION false


#include "../../engine/ecs/systems/MeshSystem.hpp"

#include "../third_party/luajit/src/lua.hpp"
#include <stdlib.h>
#include <stdio.h>




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
		meshSystem->Init(this,getAssetPath() + "models/gltf/FlightHelmet/glTF/FlightHelmet.gltf");

		 int status, result, i;
    double sum;
    lua_State *L;

    /*
     * All Lua contexts are held in this structure. We work with it almost
     * all the time.
     */
    L = luaL_newstate();

    luaL_openlibs(L); /* Load Lua libraries */

    /* Load the file containing the script we are going to run */
    status = luaL_loadfile(L, "script.lua");
    if (status) {
        /* If something went wrong, error message is at the top of */
        /* the stack */
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
        exit(1);
    }

	
    /*
     * Ok, now here we go: We pass data to the lua script on the stack.
     * That is, we first have to prepare Lua's virtual stack the way we
     * want the script to receive it, then ask Lua to run it.
     */
    lua_newtable(L);    /* We will pass a table */

    /*
     * To put values into the table, we first push the index, then the
     * value, and then call lua_rawset() with the index of the table in the
     * stack. Let's see why it's -3: In Lua, the value -1 always refers to
     * the top of the stack. When you create the table with lua_newtable(),
     * the table gets pushed into the top of the stack. When you push the
     * index and then the cell value, the stack looks like:
     *
     * <- [stack bottom] -- table, index, value [top]
     *
     * So the -1 will refer to the cell value, thus -3 is used to refer to
     * the table itself. Note that lua_rawset() pops the two last elements
     * of the stack, so that after it has been called, the table is at the
     * top of the stack.
     */
    for (i = 1; i <= 5; i++) {
        lua_pushnumber(L, i);   /* Push the table index */
        lua_pushnumber(L, i*2); /* Push the cell value */
        lua_rawset(L, -3);      /* Stores the pair in the table */
    }

    /* By what name is the script going to reference our table? */
    lua_setglobal(L, "foo");

    /* Ask Lua to run our little script */
    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result) {
        fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
        exit(1);
    }

    /* Get the returned value at the top of the stack (index -1) */
    sum = lua_tonumber(L, -1);

    printf("Script returned: %.0f\n", sum);

    lua_pop(L, 1);  /* Take the returned value out of the stack */
    lua_close(L);   /* Cya, Lua */

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

// OS specific macros for the example main entry points
#if defined(_WIN32)
// Windows entry point
																	\
VulkanExample *vulkanExample;																		\
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)						\
{																									\
	if (vulkanExample != NULL)																		\
	{																								\
		vulkanExample->handleMessages(hWnd, uMsg, wParam, lParam);									\
	}																								\
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));												\
}																									\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)									\
{																									\
	for (int32_t i = 0; i < __argc; i++) { VulkanExample::args.push_back(__argv[i]); };  			\
	vulkanExample = new VulkanExample();															\
	vulkanExample->initVulkan();																	\
	vulkanExample->setupWindow(hInstance, WndProc);													\
	vulkanExample->prepare();																		\
	vulkanExample->renderLoop();																	\
	delete(vulkanExample);																			\
	return 0;																						\
}
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
// Android entry point
																	\
VulkanExample *vulkanExample;																		\
void android_main(android_app* state)																\
{																									\
	vulkanExample = new VulkanExample();															\
	state->userData = vulkanExample;																\
	state->onAppCmd = VulkanExample::handleAppCommand;												\
	state->onInputEvent = VulkanExample::handleAppInput;											\
	androidApp = state;																				\
	vks::android::getDeviceConfig();																\
	vulkanExample->renderLoop();																	\
	delete(vulkanExample);																			\
}
#elif defined(_DIRECT2DISPLAY)
// Linux entry point with direct to display wsi
																		\
VulkanExample *vulkanExample;																		\
static void handleEvent()                                											\
{																									\
}																									\
int main(const int argc, const char *argv[])													    \
{																									\
	for (size_t i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };  				\
	vulkanExample = new VulkanExample();															\
	vulkanExample->initVulkan();																	\
	vulkanExample->prepare();																		\
	vulkanExample->renderLoop();																	\
	delete(vulkanExample);																			\
	return 0;																						\
}
#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
																	\
VulkanExample *vulkanExample;																		\
static void handleEvent(const DFBWindowEvent *event)												\
{																									\
	if (vulkanExample != NULL)																		\
	{																								\
		vulkanExample->handleEvent(event);															\
	}																								\
}																									\
int main(const int argc, const char *argv[])													    \
{																									\
	for (size_t i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };  				\
	vulkanExample = new VulkanExample();															\
	vulkanExample->initVulkan();																	\
	vulkanExample->setupWindow();					 												\
	vulkanExample->prepare();																		\
	vulkanExample->renderLoop();																	\
	delete(vulkanExample);																			\
	return 0;																						\
}
#elif (defined(VK_USE_PLATFORM_WAYLAND_KHR) || defined(VK_USE_PLATFORM_HEADLESS_EXT))
																	\
VulkanExample *vulkanExample;																		\
int main(const int argc, const char *argv[])													    \
{																									\
	for (size_t i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };  				\
	vulkanExample = new VulkanExample();															\
	vulkanExample->initVulkan();																	\
	vulkanExample->setupWindow();					 												\
	vulkanExample->prepare();																		\
	vulkanExample->renderLoop();																	\
	delete(vulkanExample);																			\
	return 0;																						\
}
#elif defined(VK_USE_PLATFORM_XCB_KHR)
																	\
VulkanExample *vulkanExample;																		\
static void handleEvent(const xcb_generic_event_t *event)											\
{																									\
	if (vulkanExample != NULL)																		\
	{																								\
		vulkanExample->handleEvent(event);															\
	}																								\
}																									\
int main(const int argc, const char *argv[])													    \
{																									\
	for (size_t i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };  				\
	vulkanExample = new VulkanExample();															\
	vulkanExample->initVulkan();																	\
	vulkanExample->setupWindow();					 												\
	vulkanExample->prepare();																		\
	vulkanExample->renderLoop();																	\
	delete(vulkanExample);																			\
	return 0;																						\
}
#elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
#if defined(VK_EXAMPLE_XCODE_GENERATED)
																	\
VulkanExample *vulkanExample;																		\
int main(const int argc, const char *argv[])														\
{																									\
	@autoreleasepool																				\
	{																								\
		for (size_t i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };				\
		vulkanExample = new VulkanExample();														\
		vulkanExample->initVulkan();																\
		vulkanExample->setupWindow(nullptr);														\
		vulkanExample->prepare();																	\
		vulkanExample->renderLoop();																\
		delete(vulkanExample);																		\
	}																								\
	return 0;																						\
}
#endif
#endif

