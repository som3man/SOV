#pragma once

#include "Instance.hpp"

namespace SOV {
	class Swapchain;

	class Image {
	public:
		friend Swapchain;

		enum class Type {
			IMAGE_1D = VK_IMAGE_TYPE_1D,
			IMAGE_2D = VK_IMAGE_TYPE_2D,
			IMAGE_3D = VK_IMAGE_TYPE_3D,
		};

		enum class Layout {
			UNDEFINED                        = VK_IMAGE_LAYOUT_UNDEFINED,
			GENERAL                          = VK_IMAGE_LAYOUT_GENERAL,
			COLOR_ATTACHMENT_OPTIMAL         = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			DEPTH_STENCIL_ATTACHMENT_OPTIMAL = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
			DEPTH_STENCIL_READ_ONLY_OPTIMAL  = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
			SHADER_READ_ONLY_OPTIMAL         = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			TRANSFER_SRC_OPTIMAL             = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			TRANSFER_DST_OPTIMAL             = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			PREINITIALIZED                   = VK_IMAGE_LAYOUT_PREINITIALIZED,
		};
		
		enum class Tiling {
			OPTIMAL = VK_IMAGE_TILING_OPTIMAL,
			LINEAR  = VK_IMAGE_TILING_LINEAR,
		};

		enum UsageFlag {
			TRANSFER_SRC             = VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
			TRANSFER_DST             = VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			SAMPLED                  = VK_IMAGE_USAGE_SAMPLED_BIT,
			STORAGE                  = VK_IMAGE_USAGE_STORAGE_BIT,
			COLOR_ATTACHMENT         = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			DEPTH_STENCIL_ATTACHMENT = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			TRANSIENT_ATTACHMENT     = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
			INPUT_ATTACHMENT         = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
			HOST_TRANSFER            = VK_IMAGE_USAGE_HOST_TRANSFER_BIT,
		};

		enum AspectFlag {
			COLOR       = VK_IMAGE_ASPECT_COLOR_BIT,
			DEPTH       = VK_IMAGE_ASPECT_DEPTH_BIT,
			STENCIL     = VK_IMAGE_ASPECT_STENCIL_BIT,
			METADATA    = VK_IMAGE_ASPECT_METADATA_BIT,
			PLANE_0_BIT = VK_IMAGE_ASPECT_PLANE_0_BIT,
			PLANE_1_BIT = VK_IMAGE_ASPECT_PLANE_1_BIT,
			PLANE_2_BIT = VK_IMAGE_ASPECT_PLANE_2_BIT,
		};

		class View {
		public:
			enum class Type {
				VIEW_1D         = VK_IMAGE_VIEW_TYPE_1D,
				VIEW_2D         = VK_IMAGE_VIEW_TYPE_2D,
				VIEW_3D         = VK_IMAGE_VIEW_TYPE_3D,
				VIEW_CUBE       = VK_IMAGE_VIEW_TYPE_CUBE,
				VIEW_1D_ARRAY   = VK_IMAGE_VIEW_TYPE_1D_ARRAY,
				VIEW_2D_ARRAY   = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
				VIEW_CUBE_ARRAY = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY,
			};

			struct Info {
				Type type;

				ComponentMapping componentMapping;

				AspectFlag aspectFlags;

				unsigned mipLevelCount;

				unsigned baseMipLevel;

				unsigned arrayLayerCount;

				unsigned baseArrayLayer;
			};

			const SOV::Image& Image;

			View(const View&) = delete;

			View& operator =(const View&) = delete;

			View(const SOV::Image& Image, const Info& info);

			View(View&& Other) noexcept : Image(Other.Image) {
				vkView = Other.vkView;

				Other.vkView = nullptr;
			}

			~View();

			operator VkImageView() const {
				return vkView;
			}

			operator bool() const {
				return vkView;
			}

		private:
			VkImageView vkView = nullptr;

			View(const SOV::Image& Image) : Image(Image) {};
		};

		struct Info {
			Type type;

			Format format;

			Extent3 extent;

			unsigned mipLevels;

			unsigned arrayLayers;

			SampleCountFlag sampleCountFlags;

			Tiling tiling;

			UsageFlag usageFlags;

			SharingMode sharingMode;

			ACTL::Array<unsigned> queueFamilyIndices;

			Memory::PropertyFlag memoryProperties;
		};

		const SOV::Device& Device;

		const Format format;

		Layout layout = Layout::UNDEFINED;

		Image(const Image&&) = delete;

		Image& operator =(const Image&) = delete;

		Image(const SOV::Device& Device, const Info& info, Layout layout) :
			Device(Device), 
			format(info.format), 
			layout(layout) {
			CreateImage(info);

			AllocateMemory(info);
		}

		Image(Image&& Other) noexcept : 
			Device(Other.Device), 
			format(Other.format),
			layout(Other.layout) {
			vkImage = Other.vkImage;

			vkDeviceMemory = Other.vkDeviceMemory;

			Other.vkImage = nullptr;
		}

		~Image();

		operator VkImage() const {
			return vkImage;
		}

		operator bool() const {
			return vkImage;
		}

		bool hasOwnMemory() const {
			return vkDeviceMemory;
		}

		VkDeviceMemory getMemory() const {
			return vkDeviceMemory;
		}

	private:
		VkImage vkImage = nullptr;

		VkDeviceMemory vkDeviceMemory = nullptr;

		Image(const SOV::Device& Device, Format format) : Device(Device), format(format) {};

		void CreateImage(const Info& info);

		void AllocateMemory(const Info& info);
	};

	class Sampler {
	public:
		enum class MipmapMode {
			NEAREST = VK_SAMPLER_MIPMAP_MODE_NEAREST,
			LINEAR  = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		};

		enum class AddressMode {
			REPEAT                 = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			MIRRORED_REPEAT        = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
			CLAMP_TO_EDGE          = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER        = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
			MIRRORED_CLAMP_TO_EDGE = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
		};

		struct Info {
			Filter minFilter, magFilter;

			MipmapMode mipmapMode;

			AddressMode addressModes[3];

			float mipLodBias;

			bool anisotropyEnable;

			float maxAnisotropy;

			bool compareEnable;

			CompareOp compareOp;

			float minLod, maxLod;

			BorderColor borderColor;

			bool unnormalizedCoordinates;
		};

		const SOV::Device& Device;

		Sampler(const Sampler&) = delete;

		Sampler& operator =(const Sampler&) = delete;

		Sampler(const SOV::Device& Device, const Info& info);

		Sampler(Sampler&& Other) noexcept : Device(Other.Device) {
			vkSampler = Other.vkSampler;

			Other.vkSampler = nullptr;
		}

		~Sampler();

		operator VkSampler() const {
			return vkSampler;
		}

		operator bool() const {
			return vkSampler;
		}

	private:
		VkSampler vkSampler = nullptr;
	};
}