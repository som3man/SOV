#pragma once

#include "Surface.hpp"

namespace SOV {
	enum class PresentMode {
		IMMEDIATE    = VK_PRESENT_MODE_IMMEDIATE_KHR,
		MAILBOX      = VK_PRESENT_MODE_MAILBOX_KHR,
		FIFO         = VK_PRESENT_MODE_FIFO_KHR,
		FIFO_RELAXED = VK_PRESENT_MODE_FIFO_RELAXED_KHR,
	};

	enum CompositeAlphaFlag {
		OPAQUE          = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		PRE_MULTIPLIED  = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		POST_MULTIPLIED = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		INHERIT         = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
	};

	class Swapchain {
	public:
		struct ImageInfo {
			Format format;

			Extent2 extent;

			unsigned arrayLayers;

			SOV::Image::UsageFlag usageFlags;

			SharingMode sharingMode;

			ACTL::Array<unsigned> queueFamilyIndices;
		};

		struct Info {
			ImageInfo imageInfo;

			unsigned minImageCount;

			Surface::TransformFlag preTransformFlags;

			CompositeAlphaFlag compositeAlphaFlags;

			PresentMode presentMode;

			bool clipped;
		};

		const SOV::Device& Device;

		Swapchain(const Swapchain&) = delete;

		Swapchain& operator =(const Swapchain&) = delete;

		Swapchain(const SOV::Device& Device) : Device(Device) {};

		Swapchain(const SOV::Device& Device, const SOV::Surface& Surface, const Info& info) : Device(Device) {
			Recreate(Surface, info);
		}

		Swapchain(Swapchain&& Other) noexcept : Device(Other.Device), Images(ACTL::move(Other.Images)) {
			vkSwapchain = Other.vkSwapchain;

			Other.vkSwapchain = nullptr;
		}

		~Swapchain();

		void Recreate(const SOV::Surface& Surface, const Info& info) {
			if (!vkSwapchain) {
				vkSwapchain = CreateSwapchain(Surface, info);

				return;
			}

			VkSwapchainKHR newSwapchain = CreateSwapchain(Surface, info);

			this->~Swapchain();

			vkSwapchain = newSwapchain;
		}

		operator VkSwapchainKHR() const {
			return vkSwapchain;
		}

		operator bool() const {
			return vkSwapchain;
		}

		const ACTL::Array<Image>& getImages() const {
			return Images;
		}

	private:
		VkSwapchainKHR vkSwapchain = nullptr;

		ACTL::Array<Image> Images = 0;

		VkSwapchainKHR CreateSwapchain(const SOV::Surface& Surface, const Info& info);
	};
}