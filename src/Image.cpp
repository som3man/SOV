#include "Source.hpp"

namespace SOV {
	Image::View::View(const SOV::Image& Image, const Info& info) : Image(Image) {
		VkImageViewCreateInfo vkInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = Image,
			.viewType = (VkImageViewType)info.type,
			.format = (VkFormat)Image.format,
			.components = (VkComponentMapping)info.componentMapping,
			.subresourceRange = {
				.aspectMask = (VkImageAspectFlags)info.aspectFlags,
				.baseMipLevel = info.baseMipLevel,
				.levelCount = info.mipLevelCount,
				.baseArrayLayer = info.baseArrayLayer,
				.layerCount = info.arrayLayerCount,
			}
		};

		VkResult result = vkCreateImageView(Image.Device, &vkInfo, nullptr, &vkView);

		if (result)
			throw Exception("Failed to create image view.", this, (Exception::Type)result);
	}

	Image::View::~View() {
		if (!vkView)
			return;

		vkDestroyImageView(Image.Device, vkView, nullptr);

		vkView = nullptr;
	}

	Image::~Image() {
		if (!vkImage)
			return;

		if (hasOwnMemory())
			vkFreeMemory(Device, vkDeviceMemory, nullptr);

		vkDestroyImage(Device, vkImage, nullptr);

		vkImage = nullptr;
	}

	void Image::CreateImage(const Info& info) {
		VkImageCreateInfo vkInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = (VkImageType)info.type,
			.format = (VkFormat)info.format,
			.extent = (VkExtent3D)info.extent,
			.mipLevels = info.mipLevels,
			.arrayLayers = info.arrayLayers,
			.samples = (VkSampleCountFlagBits)info.sampleCountFlags,
			.tiling = (VkImageTiling)info.tiling,
			.usage = (VkImageUsageFlags)info.usageFlags,
			.sharingMode = (VkSharingMode)info.sharingMode,
			.queueFamilyIndexCount = (unsigned)info.queueFamilyIndices.GetLength(),
			.pQueueFamilyIndices = info.queueFamilyIndices.begin(),
			.initialLayout = (VkImageLayout)layout
		};

		VkResult result = vkCreateImage(Device, &vkInfo, nullptr, &vkImage);

		if (result)
			throw Exception("Failed to create image.", this, (Exception::Type)result);
	}

	void Image::AllocateMemory(const Info& info) {
		VkMemoryRequirements requirements;

		vkGetImageMemoryRequirements(Device, vkImage, &requirements);

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
			throw Exception("Failed to allocate image memory.", this, (Exception::Type)result);

		vkBindImageMemory(Device, vkImage, vkDeviceMemory, 0);
	}

	Sampler::Sampler(const SOV::Device& Device, const Info& info) : Device(Device) {
		VkSamplerCreateInfo vkInfo = {
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			.magFilter = (VkFilter)info.magFilter,
			.minFilter = (VkFilter)info.minFilter,
			.mipmapMode = (VkSamplerMipmapMode)info.mipmapMode,
			.addressModeU = (VkSamplerAddressMode)info.addressModes[0],
			.addressModeV = (VkSamplerAddressMode)info.addressModes[1],
			.addressModeW = (VkSamplerAddressMode)info.addressModes[2],
			.mipLodBias = info.mipLodBias,
			.anisotropyEnable = info.anisotropyEnable,
			.maxAnisotropy = info.maxAnisotropy,
			.compareEnable = info.compareEnable,
			.compareOp = (VkCompareOp)info.compareOp,
			.minLod = info.minLod,
			.maxLod = info.maxLod,
			.borderColor = (VkBorderColor)info.borderColor,
			.unnormalizedCoordinates = info.unnormalizedCoordinates
		};

		VkResult result = vkCreateSampler(Device, &vkInfo, nullptr, &vkSampler);

		if (result)
			throw Exception("Failed to create sampler.", this, (Exception::Type)result);
	}

	Sampler::~Sampler() {
		if (!vkSampler)
			return;

		vkDestroySampler(Device, vkSampler, nullptr);

		vkSampler = nullptr;
	}
}