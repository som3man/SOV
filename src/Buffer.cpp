#include "Source.hpp"

namespace SOV {
	Buffer::~Buffer() {
		if (!vkBuffer)
			return;

		vkDestroyBuffer(Device, vkBuffer, nullptr);

		vkBuffer = nullptr;
	}

	SOV::Memory::Requirements Buffer::GetMemoryRequirements() const {
		VkMemoryRequirements vkReqs;

		vkGetBufferMemoryRequirements(Device, vkBuffer, &vkReqs);

		return {
			.size = vkReqs.size,
			.alignment = vkReqs.alignment,
			.memoryTypeBits = vkReqs.memoryTypeBits
		};
	}

	void Buffer::BindMemory(const SOV::Memory& Memory, SOV::size memoryOffset) {
		VkResult result = vkBindBufferMemory(Device, vkBuffer, Memory, memoryOffset);

		if (result)
			throw Exception("Failed to bind buffer to memory.", this, Exception::Type(result));

		this->Memory = &Memory;
	}

	void Buffer::Init(const Info& info) {
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
}