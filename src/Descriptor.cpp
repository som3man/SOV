#include "Source.hpp"

namespace SOV {
	namespace Descriptor {
		Pool::Pool(const SOV::Device& Device, const Info& info) : Device(Device) {
			VkDescriptorPoolCreateInfo vkInfo = {
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
				.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
				.maxSets = info.maxSetCount,
				.poolSizeCount = (unsigned)info.sizes.GetLength(),
				.pPoolSizes = (VkDescriptorPoolSize*)info.sizes.begin()
			};

			VkResult result = vkCreateDescriptorPool(Device, &vkInfo, nullptr, &vkPool);

			if (result)
				throw Exception("Failed to create descriptor pool.", this, (Exception::Type)result);
		}

		Pool::~Pool() {
			if (!vkPool)
				return;

			vkDestroyDescriptorPool(Device, vkPool, nullptr);

			vkPool = nullptr;
		}

		Set::Layout::Layout(const SOV::Device& Device, const ACTL::Array<Binding>& bindings) : Device(Device) {
			const unsigned bindingCount = (unsigned)bindings.GetLength();

			VkDescriptorSetLayoutBinding* vkBindings = new VkDescriptorSetLayoutBinding[bindingCount];

			for (unsigned i = 0; i < bindingCount; i++) {
				auto& binding = bindings[i];

				vkBindings[i] = {
					.binding = binding.binding,
					.descriptorType = (VkDescriptorType)binding.descriptorType,
					.descriptorCount = binding.descriptorCount,
					.stageFlags = (VkShaderStageFlags)binding.shaderStageFlags,
					.pImmutableSamplers = nullptr
				};
			}

			VkDescriptorSetLayoutCreateInfo vkInfo = {
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
				.bindingCount = bindingCount,
				.pBindings = vkBindings,
			};

			VkResult result = vkCreateDescriptorSetLayout(Device, &vkInfo, nullptr, &vkLayout);

			delete[] vkBindings;

			if (result)
				throw Exception("Failed to create descriptor set layout.", this, (Exception::Type)result);
		}

		Set::Layout::~Layout() {
			if (!vkLayout)
				return;

			vkDestroyDescriptorSetLayout(Device, vkLayout, nullptr);

			vkLayout = nullptr;
		}

		Set::Array::Array(const Descriptor::Pool& Pool, const Descriptor::Set::Layout& SetLayout, unsigned count) : 
			Pool(Pool), 
			count(count) {
			vkSets = new VkDescriptorSet[count];
			
			VkDescriptorSetLayout* vkLayouts = new VkDescriptorSetLayout[count];

			for (unsigned i = 0; i < count; i++)
				vkLayouts[i] = SetLayout;
			
			VkDescriptorSetAllocateInfo vkInfo = {
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
				.descriptorPool = Pool,
				.descriptorSetCount = count,
				.pSetLayouts = vkLayouts
			};

			VkResult result = vkAllocateDescriptorSets(Pool.Device, &vkInfo, vkSets);

			delete[] vkLayouts;

			if (result)
				throw Exception("Failed to allocate descriptor sets.", this, (Exception::Type)result);
		}

		Set::Array::Array(const Descriptor::Pool& Pool, const ACTL::Array<Descriptor::Set::Layout&>& SetLayouts) : 
			Pool(Pool), 
			count((unsigned)SetLayouts.GetLength()) {
			vkSets = new VkDescriptorSet[count];

			VkDescriptorSetLayout* vkLayouts = new VkDescriptorSetLayout[count];

			for (unsigned i = 0; i < count; i++)
				vkLayouts[i] = SetLayouts[i];

			VkDescriptorSetAllocateInfo vkInfo = {
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
				.descriptorPool = Pool,
				.descriptorSetCount = count,
				.pSetLayouts = vkLayouts
			};

			VkResult result = vkAllocateDescriptorSets(Pool.Device, &vkInfo, vkSets);

			delete[] vkLayouts;

			if (result)
				throw Exception("Failed to allocate descriptor sets.", this, (Exception::Type)result);
		}

		Set::Array::~Array() {
			if (!vkSets)
				return;

			vkFreeDescriptorSets(Pool.Device, Pool, count, vkSets);

			delete[] vkSets;

			vkSets = nullptr;
		}
	}
}