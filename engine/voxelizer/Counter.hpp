#ifndef COUNTER_HPP
#define COUNTER_HPP

#include "../../third_party/vulkan/vulkan.h"
#include <memory>

class Counter {
private:
	std::shared_ptr<VkCommandBuffer> m_buffer, m_staging_buffer;
	std::shared_ptr<VkFence> m_fence;

public:
	void Initialize(const std::shared_ptr<VkDevice> &device);
	void Reset(const std::shared_ptr<VkCommandPool> &command_pool, uint32_t value = 0);
	uint32_t Read(const std::shared_ptr<VkCommandPool> &command_pool) const;

	const std::shared_ptr<VkCommandBuffer> &GetBuffer() const { return m_buffer; }
};

#endif
