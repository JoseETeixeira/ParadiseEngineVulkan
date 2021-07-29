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