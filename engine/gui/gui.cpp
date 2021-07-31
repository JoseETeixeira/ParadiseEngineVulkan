#include "gui.h"
int WIDTH, HEIGHT;

void GUI::initImGui(float width, float height) {
		QueueFamilyIndices Indices = findQueueFamilies(g_PhysicalDevice);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.Fonts->AddFontFromFileTTF("../../Assets/Fonts/Roboto-Medium.ttf", 16.0f);

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags )
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		io.DisplaySize = ImVec2(width, height);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = g_Instance;
		init_info.PhysicalDevice = g_PhysicalDevice;
		init_info.Device = g_Device;
		init_info.QueueFamily = Indices.graphicsFamily.value();
		init_info.Queue = g_PresentQueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = g_DescriptorPool;
		init_info.Allocator = NULL;
		init_info.MinImageCount = 2;
		init_info.ImageCount = static_cast<uint32_t>(swapChainImages.size());
		init_info.CheckVkResultFn = NULL;
		ImGui_ImplVulkan_Init(&init_info, renderPass);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		VkCommandBuffer commandBuffer = beginSingleTimeCommands();
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
		endSingleTimeCommands(commandBuffer);
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}


void GUI::imGuiSetupWindow() {
    ImGuiIO& io = ImGui::GetIO();
    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    auto WindowSize = ImVec2((float)swapChainExtent.width, (float)swapChainExtent.height);
    ImGui::SetNextWindowSize(WindowSize, ImGuiCond_::ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_::ImGuiCond_FirstUseEver);
    ImGui::NewFrame();

    // render your GUI
    ImGui::Begin("Thr34d5");
    ImGui::ShowDemoWindow();
    ImGui::End();
    // Render dear imgui UI box into our window
    ImGui::Render();

    // Update and Render additional Platform Windows
    if (io.ConfigFlags)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void GUI::recreateImGuiWindow() {
    if (!isImGuiWindowCreated)
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        recreateSwapChain();
        initImGui(float(swapChainExtent.width), float(swapChainExtent.height));
        imGuiSetupWindow();
        isImGuiWindowCreated = true;
    }
}


 void GUI::mainLoop(){
    QueueFamilyIndices indices = findQueueFamilies(g_PhysicalDevice);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (!isImGuiWindowCreated) {
            imGuiSetupWindow();
            isImGuiWindowCreated = true;
        }
        createCommandBuffers();
        drawFrame();
    }
    vkDeviceWaitIdle(g_Device);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}

void GUI::drawFrame() {

		glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);

		vkWaitForFences(g_Device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		VkResult result = vkAcquireNextImageKHR(g_Device, g_SwapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			recreateImGuiWindow();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		updateUniformBuffer();

		if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(g_Device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
			//updateVideoFrame();
		}
		imagesInFlight[imageIndex] = inFlightFences[currentFrame];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(g_Device, 1, &inFlightFences[currentFrame]);

		if (vkQueueSubmit(g_Queue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { g_SwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(g_PresentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			recreateSwapChain();
			recreateImGuiWindow();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}
