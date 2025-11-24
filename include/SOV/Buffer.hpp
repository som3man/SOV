#pragma once

#include "Device.hpp"

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

		struct Info {
			SOV::size size;

			UsageFlag usageFlags;

			SharingMode sharingMode;

			ACTL::Array<unsigned> queueFamilyIndices;

			Memory::PropertyFlag memoryProperties;
		};

		const SOV::Device& Device;

		Buffer(const Buffer&) = delete;

		Buffer& operator =(const Buffer&) = delete;

		Buffer(const SOV::Device& Device, const Info& info) : Device(Device) {
			CreateBuffer(info);

			AllocateMemory(info);
		}

		Buffer(Buffer&& Other) noexcept : Device(Other.Device) {
			vkBuffer = Other.vkBuffer;

			vkDeviceMemory = Other.vkDeviceMemory;

			Other.vkBuffer = nullptr;
		}

		~Buffer();

		operator VkBuffer() const {
			return vkBuffer;
		}

		operator bool() const {
			return vkBuffer;
		}

		VkDeviceMemory getMemory() const {
			return vkDeviceMemory;
		}

		void* MapMemory() const {
			void* data;

			vkMapMemory(Device, vkDeviceMemory, 0, VK_WHOLE_SIZE, 0, &data);

			return data;
		}

		void* MapMemory(SOV::size offset, SOV::size size) const {
			void* data;

			vkMapMemory(Device, vkDeviceMemory, offset, size, 0, &data);

			return data;
		}

		void UnmapMemory() const {
			vkUnmapMemory(Device, vkDeviceMemory);
		}

	private:
		VkBuffer vkBuffer = nullptr;

		VkDeviceMemory vkDeviceMemory = nullptr;

		void CreateBuffer(const Info& info);

		void AllocateMemory(const Info& info);
	};
}