#pragma once

#include "Device.hpp"

namespace SOV {
	class Fence {
	public:
		enum Flag {
			NONE     = 0,
			SIGNALED = VK_FENCE_CREATE_SIGNALED_BIT
		};

		const SOV::Device& Device;

		Fence(const Fence&) = delete;

		Fence& operator =(const Fence&) = delete;

		Fence(const SOV::Device& Device) : Device(Device) {
			Init(Flag::NONE);
		}

		Fence(const SOV::Device& Device, Flag flags) : Device(Device) {
			Init(flags);
		}

		Fence(Fence&& Other) noexcept : Device(Other.Device) {
			vkFence = Other.vkFence;

			Other.vkFence = nullptr;
		}

		~Fence();

		operator VkFence() const {
			return vkFence;
		}

		operator bool() const {
			return vkFence;
		}

	private:
		VkFence vkFence = nullptr;

		void Init(Flag flags);
	};

	class Semaphore {
	public:
		const SOV::Device& Device;

		Semaphore(const Semaphore&) = delete;

		Semaphore& operator =(const Semaphore&) = delete;

		Semaphore(const SOV::Device& Device);

		Semaphore(Semaphore&& Other) noexcept : Device(Other.Device) {
			vkSemaphore = Other.vkSemaphore;

			Other.vkSemaphore = nullptr;
		}

		~Semaphore();

		operator VkSemaphore() const {
			return vkSemaphore;
		}

		operator bool() const {
			return vkSemaphore;
		}

	private:
		VkSemaphore vkSemaphore = nullptr;
	};
}