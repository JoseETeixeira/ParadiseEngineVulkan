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

class GUI : public VulkanRenderer{

public:


    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }



};



#endif

