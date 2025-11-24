#include "Source.hpp"

namespace SOV {
	Buffer::~Buffer() {
		if (!vkBuffer)
			return;

		vkFreeMemory(Device, vkDeviceMemory, nullptr);

		vkDestroyBuffer(Device, vkBuffer, nullptr);

		vkBuffer = nullptr;
	}

	void Buffer::CreateBuffer(const Info& info) {
		VkBufferCreateInfo vkInfo = {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = info.size,
			.usage = (VkBufferUsageFlags)info.usageFlags,
			.sharingMode = (VkSharingMode)info.sharingMode,
			.queueFamilyIndexCount = (unsigned)info.queueFamilyIndices.GetLength(),
			.pQueueFamilyIndices = info.queueFamilyIndices.begin(),
		};

		VkResult result = vkCreateBuffer(Device, &vkInfo, nullptr, &vkBuffer);

		if (result)
			throw Exception("Failed to create buffer.", this, Exception::Type(result));
	}

	void Buffer::AllocateMemory(const Info& info) {
		VkMemoryRequirements requirements;

		vkGetBufferMemoryRequirements(Device, vkBuffer, &requirements);

		VkMemoryAllocateInfo vkInfo = {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = requirements.size,
			.memoryTypeIndex = Device.PhysicalDevice.FindMemoryType(
				requirements.memoryTypeBits,
				info.memoryProperties
			).index
		};

		VkResult result = vkAllocateMemory(Device, &vkInfo, nullptr, &vkDeviceMemory);

		if (result)
			throw Exception("Failed to allocate buffer memory.", this, Exception::Type(result));

		vkBindBufferMemory(Device, vkBuffer, vkDeviceMemory, 0);
	}
}