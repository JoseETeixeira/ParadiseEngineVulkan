#include "vulkan_buffer/vulkan_buffer.h"
#include "vulkan_device/vulkan_device.h"
#include <vector>
#include <memory>
#include <set>
#include "../third_party/vulkan/vulkan.h"
#include "../third_party/vulkan_memory_allocator/vk_mem_alloc.h"

namespace vks
{
	namespace initializers
	{

		inline std::shared_ptr<vks::Buffer> CreateBuffer(const std::shared_ptr<vks::VulkanDevice> &device, VkDeviceSize size,
                                       VmaMemoryUsage memory_usage, VkBufferUsageFlags buffer_usage,
                                       const std::vector<std::shared_ptr<VkQueue>> &access_queues) {
			VkBufferCreateInfo create_info = {};
			create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			create_info.size = size;
			create_info.usage = buffer_usage;

			std::shared_ptr<vks::Buffer> ret = std::make_shared<vks::Buffer>();
			ret->device = device->logicalDevice;
			ret->size = create_info.size;

			std::set<uint32_t> queue_family_set;
			for (auto &i : access_queues)
				queue_family_set.insert(device->getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT));
			std::vector<uint32_t> queue_families{queue_family_set.begin(), queue_family_set.end()};

			VkBufferCreateInfo new_info{create_info};
			if (queue_families.size() <= 1) {
				new_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			} else {
				new_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
				new_info.queueFamilyIndexCount = queue_families.size();
				new_info.pQueueFamilyIndices = queue_families.data();
			}

			VmaAllocationCreateInfo alloc_create_info = {};
			alloc_create_info.usage = memory_usage;

			if (vmaCreateBuffer(device->GetAllocatorHandle(), &new_info, &alloc_create_info, &ret->buffer, &ret->m_allocation,
								nullptr) != VK_SUCCESS)
				return nullptr;

			return ret;
		}
    }
}