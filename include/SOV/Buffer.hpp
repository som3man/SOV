#pragma once

#include "Image.hpp"

namespace SOV {
	class Buffer {
	public:
		enum UsageFlag {
			TRANSFER_SRC          = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			TRANSFER_DST          = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			UNIFORM_TEXEL_BUFFER  = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
			STORAGE_TEXEL_BUFFER  = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT,
			UNIFORM_BUFFER        = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			STORAGE_BUFFER        = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
			INDEX_BUFFER          = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VERTEX_BUFFER         = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			INDIRECT_BUFFER       = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
			SHADER_DEVICE_ADDRESS = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
		};

		struct ImageCopyInfo {
			SOV::size bufferOffset;

			unsigned bufferRowLength;

			unsigned bufferImageHeight;

			Image::SubresourceLayers imageSubresourceLayers;

			Offset3 imageOffset;

			Extent3 imageExtent;
		};
		
		struct CopyInfo {
			SOV::size srcOffset;

			SOV::size dstOffset;

			SOV::size size;
		};

		struct Info {
			SOV::size size;

			UsageFlag usageFlags;

			SharingMode sharingMode;

			ACTL::Array<unsigned> queueFamilyIndices;
		};

		const SOV::Device& Device;

		Buffer(const Buffer&) = delete;

		Buffer& operator =(const Buffer&) = delete;

		Buffer(const SOV::Device& Device, const Info& info) : Device(Device) {
			Init(info);
		}

		Buffer(const SOV::Memory& Memory, SOV::size memoryOffset, const Info& info) : Device(Memory.Device) {
			Init(info);

			BindMemory(Memory, memoryOffset);
		}

		Buffer(Buffer&& Other) noexcept : Device(Other.Device), Memory(Other.Memory) {
			vkBuffer = Other.vkBuffer;

			Other.vkBuffer = nullptr;
		}

		~Buffer();

		SOV::Memory::Requirements GetMemoryRequirements() const;

		void BindMemory(const SOV::Memory& Memory, SOV::size memoryOffset);

		void Recreate(const Info& info) {
			Memory = nullptr;

			this->~Buffer();

			Init(info);
		}

		operator VkBuffer() const {
			return vkBuffer;
		}

		operator bool() const {
			return vkBuffer;
		}

		const SOV::Memory* getMemory() const {
			return Memory;
		}

	private:
		const SOV::Memory* Memory = nullptr;

		VkBuffer vkBuffer = nullptr;

		void Init(const Info& info);
	};
}