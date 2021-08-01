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
#include "../vulkan_buffer/vulkan_buffer.h"
#include "../vulkan_device/vulkan_device.h"



class GUI : public VulkanRenderer{

public:


    void run() {
        initWindow();
        initVulkan();
        initImGui(float(swapChainExtent.width), float(swapChainExtent.height));
        mainLoop();
        cleanup();
    }


private:
    void initImGui(float width, float height);
    void mainLoop() override;
    void imGuiSetupWindow();
    void recreateImGuiWindow();
    void drawFrame() override;
    void draw_callback(const ImDrawList* parent_list);

    ImGui_ImplVulkanH_Window g_MainWindowData;
    int g_MinImageCount = 2;
    VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
    VkRenderPass imgui_renderpass;
    bool g_SwapChainRebuild = false;
    int32_t vertexCount = 0;
    int32_t indexCount = 0;
    vks::Buffer vertexBuffer;
    vks::Buffer indexBuffer;
    vks::VulkanDevice *device;
    

  

};



#endif

