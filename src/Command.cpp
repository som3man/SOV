#include "Source.hpp"

namespace SOV {
	namespace Command {
		Pool::~Pool() {
			if (!vkPool)
				return;

			vkDestroyCommandPool(Device, vkPool, nullptr);

			vkPool = nullptr;
		}

		void Pool::Init() {
			VkCommandPoolCreateInfo vkInfo = {
				.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
			};

			VkResult result = vkCreateCommandPool(Device, &vkInfo, nullptr, &vkPool);

			if (result)
				throw Exception("Failed to create command pool.", this, (Exception::Type)result);
		}

		void Buffer::Begin(UsageFlag usageFlags) const {
			VkCommandBufferBeginInfo info = {
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				.flags = (VkCommandBufferUsageFlags)usageFlags
			};

			VkResult result = vkBeginCommandBuffer(vkBuffer, &info);

			if (result)
				throw Exception("Failed to begin command buffer recording.", this, (Exception::Type)result);
		}

		void Buffer::End() const {
			VkResult result = vkEndCommandBuffer(vkBuffer);

			if (result)
				throw Exception("Failed to end command buffer recording.", this, (Exception::Type)result);
		}

		Buffer::Array::~Array() {
			if (!vkBuffers)
				return;

			vkFreeCommandBuffers(Pool.Device, Pool, count, vkBuffers);

			delete[] vkBuffers;

			vkBuffers = nullptr;
		}

		void Buffer::Array::Init() {
			VkCommandBufferAllocateInfo info = {
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				.commandPool = Pool,
				.commandBufferCount = count,
			};

			vkBuffers = new VkCommandBuffer[count];

			VkResult result = vkAllocateCommandBuffers(Pool.Device, &info, vkBuffers);

			if (result)
				throw Exception("Failed to allocate command buffers.", this, (Exception::Type)result);
		}
	}
}