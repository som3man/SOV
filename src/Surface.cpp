#include "Source.hpp"

namespace SOV {
	Surface::~Surface() {
		if (!vkSurface)
			return;

		vkDestroySurfaceKHR(PhysicalDevice.Instance, vkSurface, nullptr);

		vkSurface = nullptr;
	}

	Surface::Capabilities Surface::GetCapabilities(const SOV::PhysicalDevice& PhysicalDevice, VkSurfaceKHR vkSurface) {
		VkSurfaceCapabilitiesKHR vkCapabilities;

		VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, vkSurface, &vkCapabilities);

		if (result)
			throw Exception("Failed to get surface capabilities.", vkSurface, (Exception::Type)result);

		return {
			.minImageCount = vkCapabilities.minImageCount,
			.maxImageCount = vkCapabilities.maxImageCount,
			.currentExtent = {
				.width = vkCapabilities.currentExtent.width,
				.height = vkCapabilities.currentExtent.height,
			},
			.minImageExtent = {
				.width = vkCapabilities.minImageExtent.width,
				.height = vkCapabilities.minImageExtent.height,
			},
			.maxImageExtent = {
				.width = vkCapabilities.maxImageExtent.width,
				.height = vkCapabilities.maxImageExtent.height,
			},
			.imageUsageFlags = (Image::UsageFlag)vkCapabilities.supportedUsageFlags
		};
	}
}