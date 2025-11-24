#include "Source.hpp"

namespace SOV {
	Fence::~Fence() {
		if (!vkFence)
			return;

		vkDestroyFence(Device, vkFence, nullptr);

		vkFence = nullptr;
	}

	void Fence::Init(Flag flags) {
		VkFenceCreateInfo vkInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = (VkFenceCreateFlags)flags
		};

		VkResult result = vkCreateFence(Device, &vkInfo, nullptr, &vkFence);

		if (result)
			throw Exception("Failed to create fence.", this, (Exception::Type)result);
	}

	Semaphore::Semaphore(const SOV::Device& Device) : Device(Device) {
		VkSemaphoreCreateInfo vkInfo = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};

		VkResult result = vkCreateSemaphore(Device, &vkInfo, nullptr, &vkSemaphore);

		if (result)
			throw Exception("Failed to create semaphore.", this, (Exception::Type)result);
	}

	Semaphore::~Semaphore() {
		if (!vkSemaphore)
			return;

		vkDestroySemaphore(Device, vkSemaphore, nullptr);

		vkSemaphore = nullptr;
	}
}