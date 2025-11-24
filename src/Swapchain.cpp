#include "Source.hpp"

namespace SOV {
	Swapchain::Swapchain(const SOV::Device& Device, const SOV::Surface& Surface, const Info& info) : Device(Device) {
		VkSwapchainCreateInfoKHR vkInfo = {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = Surface,
			.minImageCount = info.minImageCount,
			.imageFormat = (VkFormat)info.imageInfo.format,
			.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
			.imageExtent = {
				.width = info.imageInfo.extent.width,
				.height = info.imageInfo.extent.height,
			},
			.imageArrayLayers = info.imageInfo.arrayLayers,
			.imageUsage = (VkImageUsageFlags)info.imageInfo.usageFlags,
			.imageSharingMode = (VkSharingMode)info.imageInfo.sharingMode,
			.queueFamilyIndexCount = (unsigned)info.imageInfo.queueFamilyIndices.GetLength(),
			.pQueueFamilyIndices = info.imageInfo.queueFamilyIndices.begin(),
			.preTransform = (VkSurfaceTransformFlagBitsKHR)info.preTransformFlags,
			.compositeAlpha = (VkCompositeAlphaFlagBitsKHR)info.compositeAlphaFlags,
			.presentMode = (VkPresentModeKHR)info.presentMode,
			.clipped = (VkBool32)info.clipped
		};

		VkResult result = vkCreateSwapchainKHR(Device, &vkInfo, nullptr, &vkSwapchain);

		if (result)
			throw Exception("Failed to create swapchain.", this, (Exception::Type)result);

		unsigned imageCount;

		vkGetSwapchainImagesKHR(Device, vkSwapchain, &imageCount, nullptr);

		Images = imageCount;

		VkImage* vkImages = new VkImage[imageCount];

		vkGetSwapchainImagesKHR(Device, vkSwapchain, &imageCount, vkImages);

		for (unsigned i = 0; i < imageCount; i++) {
			auto& Image = Images.EmplaceBack(SOV::Image(Device, info.imageInfo.format));

			Image.vkImage = vkImages[i];
		}

		delete[] vkImages;
	}

	Swapchain::~Swapchain() {
		if (!vkSwapchain)
			return;

		for (auto& Image : Images)
			Image.vkImage = nullptr;

		vkDestroySwapchainKHR(Device, vkSwapchain, nullptr);

		vkSwapchain = nullptr;
	}
}