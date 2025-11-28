#include "Source.hpp"

namespace SOV {
	Image::View::~View() {
		if (!vkView)
			return;

		vkDestroyImageView(Image.Device, vkView, nullptr);

		vkView = nullptr;
	}

	void Image::View::Recreate(const Info& info) {
		this->~View();

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

	Image::~Image() {
		if (!vkImage)
			return;

		vkDestroyImage(Device, vkImage, nullptr);

		vkImage = nullptr;
	}

	SOV::Memory::Requirements Image::GetMemoryRequirements() const {
		VkMemoryRequirements vkReqs;

		vkGetImageMemoryRequirements(Device, vkImage, &vkReqs);

		return {
			.size = vkReqs.size,
			.alignment = vkReqs.alignment,
			.memoryTypeBits = vkReqs.memoryTypeBits
		};
	}

	void Image::BindMemory(const SOV::Memory& Memory, SOV::size memoryOffset) {
		if (this->Memory == SOV::Memory::External)
			throw Exception("This image is binded to an API controlled memory.", this, Exception::Type::OTHER);

		VkResult result = vkBindImageMemory(Device, vkImage, Memory, memoryOffset);

		if (result)
			throw Exception("Failed to bind image to memory.", this, Exception::Type(result));

		this->Memory = &Memory;
	}

	void Image::Init(const Info& info) {
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

	Sampler::~Sampler() {
		if (!vkSampler)
			return;

		vkDestroySampler(Device, vkSampler, nullptr);

		vkSampler = nullptr;
	}

	void Sampler::Recreate(const Info& info) {
		this->~Sampler();

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
}