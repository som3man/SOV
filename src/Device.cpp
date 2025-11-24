#include "Source.hpp"

namespace SOV {
	const Memory::Type& PhysicalDevice::FindMemoryType(unsigned filter, Memory::PropertyFlag memoryProperties) const {
		for (auto& type : info.memoryTypes)
			if ((filter & (1 << type.index)) && (type.properties & memoryProperties) == memoryProperties)
				return type;

		throw Exception("Failed to find suitable memory type.", this, Exception::Type::OTHER);
	}

	PhysicalDevice::PhysicalDevice(const SOV::Instance& Instance, VkPhysicalDevice vkPhysicalDevice):
		Instance(Instance),
		vkPhysicalDevice(vkPhysicalDevice) {
		VkPhysicalDeviceProperties properties;

		vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);

		info.type = (Type)properties.deviceType;

		info.name = properties.deviceName;

		info.apiVersion = {
			.major = (unsigned short)VK_VERSION_MAJOR(properties.apiVersion),
			.minor = (unsigned short)VK_VERSION_MINOR(properties.apiVersion),
			.patch = (unsigned short)VK_VERSION_PATCH(properties.apiVersion),
		};

		VkPhysicalDeviceMemoryProperties memoryProperties;

		vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memoryProperties);

		const unsigned typeCount = memoryProperties.memoryTypeCount;

		info.memoryTypes.SetCapacity(typeCount);

		for (unsigned i = 0; i < typeCount; i++) {
			Memory::Type& memoryType = info.memoryTypes.EmplaceBack(Memory::Type(*this));

			memoryType.index = i;

			memoryType.properties = (Memory::PropertyFlag)memoryProperties.memoryTypes[i].propertyFlags;
		}

		unsigned familyCount;

		vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &familyCount, nullptr);

		VkQueueFamilyProperties* familyProperties = new VkQueueFamilyProperties[familyCount];

		vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &familyCount, familyProperties);

		info.queueFamilies.SetCapacity(familyCount);

		for (unsigned i = 0; i < familyCount; i++) {
			auto& vkFamily = familyProperties[i];

			Queue::Family& family = info.queueFamilies.EmplaceBack(Queue::Family(*this));

			family.index = i;

			family.flags = (Queue::Family::Flag)vkFamily.queueFlags;

			family.queueCount = vkFamily.queueCount;
		}

		delete[] familyProperties;
	}

	Device::Device(const SOV::PhysicalDevice& PhysicalDevice, const ACTL::Array<Extension>& extensions) :
		PhysicalDevice(PhysicalDevice) {
		const unsigned familyCount = (unsigned)PhysicalDevice.info.queueFamilies.GetLength();

		Queues.SetCapacity(familyCount);

		for (unsigned i = 0; i < familyCount; i++) {
			const unsigned queueCount = (unsigned)PhysicalDevice.info.queueFamilies[i].queueCount;

			auto& family = Queues.EmplaceBack(ACTL::Array<Queue>(queueCount));

			for (unsigned j = 0; j < queueCount; j++) {
				SOV::Queue& Queue = family.EmplaceBack(SOV::Queue(PhysicalDevice.info.queueFamilies[i]));

				Queue.index = j;

				Queue.priority = 1.0f;
			}
		}

		Init(extensions);
	}

	Device::Device(const SOV::PhysicalDevice& PhysicalDevice, const ACTL::Array<Extension>& extensions, const ACTL::Array<ACTL::Array<float>>& queuePriorities) :
		PhysicalDevice(PhysicalDevice) {
		const unsigned familyCount = (unsigned)queuePriorities.GetLength();

		Queues.SetCapacity(familyCount);

		for (unsigned i = 0; i < familyCount; i++) {
			const unsigned queueCount = (unsigned)queuePriorities[i].GetLength();

			auto& family = Queues.EmplaceBack(ACTL::Array<Queue>(queueCount));

			for (unsigned j = 0; j < queueCount; j++) {
				SOV::Queue& Queue = family.EmplaceBack(SOV::Queue(PhysicalDevice.info.queueFamilies[i]));

				Queue.index = j;

				Queue.priority = queuePriorities[i][j];
			}
		}

		Init(extensions);
	}

	Device::~Device() {
		if (!vkDevice)
			return;

		vkDestroyDevice(vkDevice, nullptr);

		vkDevice = nullptr;
	}

	void Device::Init(const ACTL::Array<Extension>& extensions) {
		const unsigned familyCount = Queues.GetLength();

		VkDeviceQueueCreateInfo* queueInfos = new VkDeviceQueueCreateInfo[familyCount];

		for (unsigned i = 0; i < familyCount; i++) {
			const unsigned queueCount = (unsigned)Queues[i].GetLength();

			float* priorities = new float[queueCount];

			for (unsigned j = 0; j < queueCount; j++)
				priorities[j] = Queues[i][j].priority;

			queueInfos[i] = {
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = i,
				.queueCount = queueCount,
				.pQueuePriorities = priorities,
			};
		}

		const unsigned extensionCount = (unsigned)extensions.GetLength();

		const char** vkExtensions = new const char* [extensionCount];

		for (unsigned i = 0; i < extensionCount; i++)
			vkExtensions[i] = extensions[i];

		VkDeviceCreateInfo vkInfo = {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.queueCreateInfoCount = familyCount,
			.pQueueCreateInfos = queueInfos,
			.enabledExtensionCount = extensionCount,
			.ppEnabledExtensionNames = vkExtensions,
		};

		VkResult result = vkCreateDevice(PhysicalDevice, &vkInfo, nullptr, &vkDevice);

		if (result)
			throw Exception("Failed to create logical device.", this, (Exception::Type)result);

		delete[] vkExtensions;

		for (unsigned i = 0; i < familyCount; i++)
			delete[] queueInfos[i].pQueuePriorities;

		delete[] queueInfos;
	}
}