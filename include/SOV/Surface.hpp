#pragma once

#include "Image.hpp"

namespace SOV {
	class Surface {
	public:
		enum TransformFlag {
			IDENTITY                     = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			ROTATE_90                    = VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR,
			ROTATE_180                   = VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR,
			ROTATE_270                   = VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR,
			HORIZONTAL_MIRROR            = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR,
			HORIZONTAL_MIRROR_ROTATE_90  = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR,
			HORIZONTAL_MIRROR_ROTATE_180 = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR,
			HORIZONTAL_MIRROR_ROTATE_270 = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR,
			INHERIT                      = VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR,
		};

		struct Capabilities {
			unsigned minImageCount, maxImageCount;

			Extent2 currentExtent, minImageExtent, maxImageExtent;

			Image::UsageFlag imageUsageFlags;
		};

		const SOV::PhysicalDevice& PhysicalDevice;

		const Capabilities capabilities;

		Surface(const Surface&&) = delete;

		Surface& operator =(const Surface&) = delete;

		Surface(const SOV::PhysicalDevice& PhysicalDevice, VkSurfaceKHR vkSurface) : 
			PhysicalDevice(PhysicalDevice),
			capabilities(GetCapabilities(PhysicalDevice, vkSurface)) {
			this->vkSurface = vkSurface;
		}

		Surface(Surface&& Other) noexcept : 
			PhysicalDevice(Other.PhysicalDevice),
			capabilities(Other.capabilities) {
			vkSurface = Other.vkSurface;

			Other.vkSurface = nullptr;
		}

		~Surface();

		operator VkSurfaceKHR() const {
			return vkSurface;
		}

		operator bool() const {
			return vkSurface;
		}

	private:
		VkSurfaceKHR vkSurface = nullptr;

		static Capabilities GetCapabilities(const SOV::PhysicalDevice& PhysicalDevice, VkSurfaceKHR vkSurface);
	};
}