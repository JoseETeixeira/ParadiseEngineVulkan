/*
* ParadiseEngine - Vulkan (https://github.com/JoseETeixeira/ParadiseEngineVulkan)
*
* Copyright (C) 2021 by José Eduardo da Silva Teixeira Junior - www.developmentparadise.com
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/


#pragma once
#ifndef GUI_H
#define GUI_H

#include "../vulkan_renderer/vulkan_renderer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#define IMGUI_UNLIMITED_FRAME_RATE



class GUI : public VulkanRenderer{

public:


    void run() {
        initWindow();
        initVulkan();
        initImGui();
        mainLoop();
        cleanup();
    }


private:
    void initImGui();
    void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

    ImGui_ImplVulkanH_Window g_MainWindowData;
    int g_MinImageCount = 2;
    VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
    VkRenderPass imgui_renderpass;


};



#endif

