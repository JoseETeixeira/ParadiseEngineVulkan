/*
* ParadiseEngine - Vulkan (https://github.com/JoseETeixeira/ParadiseEngineVulkan)
*
* Copyright (C) 2021 by José Eduardo da Silva Teixeira Junior - www.developmentparadise.com
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "vulkan_renderer/vulkan_renderer.h"

int main() {
    VulkanRenderer app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}