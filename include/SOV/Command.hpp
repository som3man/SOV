#pragma once

#include "Buffer.hpp"
#include "Descriptor.hpp"
#include "Sync.hpp"

namespace SOV {
	namespace Command {
		class Pool {
		public:
			const SOV::Device& Device;

			Pool& operator =(const Pool&) = delete;

			Pool(const SOV::Device& Device) : Device(Device) {
				Init();
			}

			Pool(const Pool& Other) : Device(Other.Device) {
				Init();
			}

			Pool(Pool&& Other) noexcept : Device(Other.Device) {
				vkPool = Other.vkPool;

				Other.vkPool = nullptr;
			}

			~Pool();

			operator VkCommandPool() const {
				return vkPool;
			}

			operator bool() const {
				return vkPool;
			}

		private:
			VkCommandPool vkPool = nullptr;

			void Init();
		};

		class Buffer {
		public:
			class Array;

			friend Array;
			
			enum UsageFlag {
				NONE                 = 0,
				ONE_TIME_SUBMIT      = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
				RENDER_PASS_CONTINUE = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
				SIMULTANEOUS_USE     = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			};

			Buffer(const Buffer& Other) {
				operator=(Other);
			}

			Buffer(Buffer&& Other) noexcept {
				operator=(ACTL::move(Other));
			}

			Buffer& operator =(const Buffer& Other) {
				vkBuffer = Other.vkBuffer;

				return *this;
			}

			Buffer& operator =(Buffer&& Other) noexcept {
				vkBuffer = Other.vkBuffer;

				Other.vkBuffer = nullptr;

				return *this;
			}

			~Buffer() {};

			operator VkCommandBuffer() const {
				return vkBuffer;
			}

			operator bool() const {
				return vkBuffer;
			}

			void Begin(UsageFlag usageFlags) const;

			void End() const;

			void Draw(
				unsigned vertexCount,
				unsigned instanceCount,
				unsigned firstVertex,
				unsigned firstInstance
			) const {
				vkCmdDraw(
					vkBuffer,
					vertexCount,
					instanceCount,
					firstVertex,
					firstInstance
				);
			}

			void DrawIndexed(
				unsigned indexCount,
				unsigned instanceCount,
				unsigned firstIndex,
				unsigned vertexOffset,
				unsigned firstInstance
			) const {
				vkCmdDrawIndexed(
					vkBuffer,
					indexCount,
					instanceCount,
					firstIndex,
					vertexOffset,
					firstInstance
				);
			}

			void DrawIndexedIndirect(
				const SOV::Buffer& Buffer,
				SOV::size offset,
				unsigned drawCount,
				unsigned stride
			) const {
				vkCmdDrawIndexedIndirect(
					vkBuffer,
					Buffer,
					offset,
					drawCount,
					stride
				);
			}

		private:
			VkCommandBuffer vkBuffer;

			Buffer(VkCommandBuffer vkBuffer) : vkBuffer(vkBuffer) {};
		};

		class Buffer::Array {
		public:
			const Command::Pool& Pool;

			const unsigned count;

			Array(const Command::Pool& Pool, unsigned count) : Pool(Pool), count(count) {
				Init();
			}

			Array(const Array& Other) : Pool(Other.Pool), count(Other.count) {
				Init();
			}

			Array(Array&& Other) noexcept : Pool(Other.Pool), count(Other.count) {
				vkBuffers = Other.vkBuffers;

				Other.vkBuffers = nullptr;
			}

			~Array();

			operator VkCommandBuffer* () const {
				return vkBuffers;
			}

			operator bool() const {
				return vkBuffers;
			}

			Buffer operator [](unsigned index) const {
				return vkBuffers[index];
			}

			const Buffer* begin() const {
				return (Buffer*)vkBuffers;
			}

			const Buffer* end() const {
				return (Buffer*)(vkBuffers + count);
			}

		private:
			VkCommandBuffer* vkBuffers = nullptr;

			void Init();
		};
	}
}