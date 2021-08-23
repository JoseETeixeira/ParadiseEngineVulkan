/*
* Paradise Engine
*
* Copyright (C) 2021 by José Eduardo da Silva Teixeira Junior - https://developmentparadise.com
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/


/*
	Entry point for the Paradise Engine
*/



#include "../../engine/ecs/components/Camera.hpp"
#include "../../engine/ecs/components/Transform.hpp"
#include "../../engine/ecs/components/Renderable.hpp"

#include "../../engine/ecs/Coordinator.hpp"

#include "../../engine/ecs/systems/CameraControlSystem.hpp"
#include "../../engine/vulkan_example_base/vulkan_example_base.h"


#define ENABLE_VALIDATION false


#include "../../engine/ecs/systems/MeshSystem.hpp"

#include "../lua_bindings/lua_bindings.h"

#ifdef _WIN32
extern "C"{
	#include "../Lua542/include/lua.h"
	#include "../Lua542/include/lauxlib.h"
	#include "../Lua542/include/lualib.h"
}
#else
	#include "../third_party/luajit/src/lua.hpp"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



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
		gCoordinator.RegisterComponent<Renderable>();

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
			signature.set(gCoordinator.GetComponentType<Renderable>());
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





	void setupMeshes(lua_State *L){
		lua_pushnil(L);

		while(lua_next(L, -2) != 0)
		{
			
			if(lua_isstring(L, -1)){
				const char* variable = lua_tostring(L, -2);
				const char* path = lua_tostring(L, -1);

				printf("Is string\n");
			
				printf("%s = %s\n", variable, path);

				if(strcmp(variable,"mesh") == 0){
				

					meshSystem->Init(this,getAssetPath() + path);
				}
				
				
			}
			else if(lua_isnumber(L, -1)){
				printf("Is number\n");
				printf("%s = %d\n", lua_tostring(L, -2), lua_tonumber(L, -1));
			}
			
			else if(lua_istable(L, -1)){
				printf("Is table\n");
				setupMeshes(L);
			}
			

			lua_pop(L, 1);
		}
	}

	void setupLua(){

		lua_State *L;

		/*
		* All Lua contexts are held in this structure. We work with it almost
		* all the time.
		*/
		L = luaL_newstate();

		luaL_openlibs(L); /* Load Lua libraries */

		// Load file.
		if(luaL_loadfile(L, "editor.lua") || lua_pcall(L, 0, 0, 0))
		{
			printf("Cannot run file\n");
			return;
		}

		// Print table contents.
		lua_getglobal(L, "world");


		lua_pushstring (L, "meshes");
		lua_gettable (L, -2);
		lua_pushnil(L);
		lua_next(L, -2);
		setupMeshes(L);
		lua_pop (L, 1);


		lua_close(L);

	}
	

	void prepare()
	{
		
		VulkanExampleBase::prepare();
		setupLua();
	
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

