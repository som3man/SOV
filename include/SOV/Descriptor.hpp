#pragma once

#include "Shader.hpp"

namespace SOV {
	namespace Descriptor {
		enum class Type {
			SAMPLER                = VK_DESCRIPTOR_TYPE_SAMPLER,
			COMBINED_IMAGE_SAMPLER = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			SAMPLED_IMAGE          = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
			STORAGE_IMAGE          = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
			UNIFORM_TEXEL_BUFFER   = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
			STORAGE_TEXEL_BUFFER   = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
			UNIFORM_BUFFER         = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			STORAGE_BUFFER         = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			UNIFORM_BUFFER_DYNAMIC = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
			STORAGE_BUFFER_DYNAMIC = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
			INPUT_ATTACHMENT       = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
		};

		class Pool {
		public:
			struct Size {
				Descriptor::Type descriptorType;

				unsigned descriptorCount;
			};

			struct Info {
				unsigned maxSetCount;

				ACTL::Array<Size> sizes;
			};

			const SOV::Device& Device;

			Pool(const Pool&) = delete;

			Pool& operator =(const Pool&) = delete;

			Pool(const SOV::Device& Device, const Info& info);

			Pool(Pool&& Other) noexcept : Device(Other.Device) {
				vkPool = Other.vkPool;

				Other.vkPool = nullptr;
			}

			~Pool();

			operator VkDescriptorPool() const {
				return vkPool;
			}

			operator bool() const {
				return vkPool;
			}

		private:
			VkDescriptorPool vkPool = nullptr;
		};

		class Set {
		public:
			class Layout;

			class Array;

			friend Array;

			Set(const Set& Other) {
				operator=(Other);
			}

			Set(Set&& Other) noexcept {
				operator=(ACTL::move(Other));
			}

			Set& operator =(const Set& Other) {
				vkSet = Other.vkSet;

				return *this;
			}

			Set& operator =(Set&& Other) noexcept {
				vkSet = Other.vkSet;

				return *this;
			}

			operator VkDescriptorSet() const {
				return vkSet;
			}

			operator bool() const {
				return vkSet;
			}

		private:
			VkDescriptorSet vkSet = nullptr;

			Set(VkDescriptorSet vkSet) : vkSet(vkSet) {};
		};

		class Set::Layout {
		public:
			struct Binding {
				unsigned binding;

				Descriptor::Type descriptorType;

				unsigned descriptorCount;

				Shader::StageFlag shaderStageFlags;
			};

			const SOV::Device& Device;

			Layout(const Layout&) = delete;

			Layout& operator =(const Layout&) = delete;

			Layout(const SOV::Device& Device, const ACTL::Array<Binding>& bindings);

			Layout(Layout&& Other) noexcept : Device(Other.Device) {
				vkLayout = Other.vkLayout;

				Other.vkLayout = nullptr;
			}

			~Layout();

			operator VkDescriptorSetLayout() const {
				return vkLayout;
			}

			operator bool() const {
				return vkLayout;
			}

		private:
			VkDescriptorSetLayout vkLayout = nullptr;
		};

		class Set::Array {
		public:
			const Descriptor::Pool& Pool;

			const unsigned count;

			Array(const Array&) = delete;

			Array& operator =(const Array&) = delete;

			Array(const Descriptor::Pool& Pool, const Descriptor::Set::Layout& SetLayout, unsigned count);

			Array(const Descriptor::Pool& Pool, const ACTL::Array<Descriptor::Set::Layout*>& SetLayouts);

			Array(Array&& Other) noexcept : Pool(Other.Pool), count(Other.count) {
				vkSets = Other.vkSets;

				Other.vkSets = nullptr;
			}

			~Array();

			operator VkDescriptorSet* () const {
				return vkSets;
			}

			operator bool() const {
				return vkSets;
			}

			Set operator [](unsigned index) const {
				return vkSets[index];
			}

			const Set* begin() const {
				return (Set*)vkSets;
			}

			const Set* end() const {
				return (Set*)(vkSets + count);
			}

		private:
			VkDescriptorSet* vkSets = nullptr;
		};
	}
}