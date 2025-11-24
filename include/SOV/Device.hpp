#pragma once

#include "Defines.hpp"

namespace SOV {
	class Instance;

	class PhysicalDevice;

	class Device;

	class Fence;

	namespace Memory {
		enum PropertyFlag {
			NONE             = 0,
			DEVICE_LOCAL     = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			HOST_VISIBLE     = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			HOST_COHERENT    = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			HOST_CACHED      = VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
			LAZILY_ALLOCATED = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT,
			PROTECTED        = VK_MEMORY_PROPERTY_PROTECTED_BIT,
		};
		
		struct Type {
			friend PhysicalDevice;

			const SOV::PhysicalDevice& PhysicalDevice;

			PropertyFlag properties = PropertyFlag::NONE;

			unsigned index = 0;

			Type(const Type&) = delete;

			Type& operator =(const Type&) = delete;

			Type(Type&& other) noexcept : PhysicalDevice(other.PhysicalDevice) {
				properties = other.properties;

				index = other.index;
			}

			~Type() {};

		private:
			Type(const SOV::PhysicalDevice& Device) : PhysicalDevice(PhysicalDevice) {};
		};
	}

	class Queue {
	public:
		friend Device;

		struct Family;
		
		const Family& family;

		unsigned index = 0;

		float priority = 1.0f;

		Queue& operator =(const Queue&) = delete;

		Queue(const Queue& Other) : family(Other.family) {
			index = Other.index;

			priority = Other.priority;

			vkQueue = Other.vkQueue;
		}

		Queue(Queue&& Other) noexcept : family(Other.family) {
			index = Other.index;

			priority = Other.priority;

			vkQueue = Other.vkQueue;
		}

		~Queue() {};

		operator VkQueue() const {
			return vkQueue;
		}

	private:
		VkQueue vkQueue = nullptr;

		Queue(const Family& family) : family(family) {};
	};

	struct Queue::Family {
		friend PhysicalDevice;

		enum Flag {
			NONE           = 0,
			GRAPHICS       = VK_QUEUE_GRAPHICS_BIT,
			COMPUTE        = VK_QUEUE_COMPUTE_BIT,
			TRANSFER       = VK_QUEUE_TRANSFER_BIT,
			SPARSE_BINDING = VK_QUEUE_SPARSE_BINDING_BIT,
			PROTECTED      = VK_QUEUE_PROTECTED_BIT,
		};

		const SOV::PhysicalDevice& PhysicalDevice;

		unsigned index = 0;

		unsigned queueCount = 0;

		Flag flags = Flag::NONE;

		Family(const Family&) = delete;

		Family& operator =(const Family&) = delete;

		Family(Family&& other) noexcept : PhysicalDevice(other.PhysicalDevice) {
			index = other.index;

			flags = other.flags;

			queueCount = other.queueCount;
		}

		~Family() {};

	private:
		Family(const SOV::PhysicalDevice& Device) : PhysicalDevice(PhysicalDevice) {};
	};

	class PhysicalDevice {
	public:
		friend Instance;

		enum class Type {
			OTHER          = VK_PHYSICAL_DEVICE_TYPE_OTHER,
			INTEGRATED_GPU = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
			DISCRETE_GPU   = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
			VIRTUAL_GPU    = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
			CPU            = VK_PHYSICAL_DEVICE_TYPE_CPU,
		};

		struct Info {
			Type type;

			ACTL::String<char> name;

			Version apiVersion;

			ACTL::Array<Memory::Type> memoryTypes;

			ACTL::Array<Queue::Family> queueFamilies;
		};

		const SOV::Instance& Instance;

		Info info;

		PhysicalDevice(const PhysicalDevice&) = delete;

		PhysicalDevice& operator =(const PhysicalDevice&) = delete;

		PhysicalDevice(PhysicalDevice&& other) noexcept : Instance(other.Instance), info(ACTL::move(other.info)) {
			vkPhysicalDevice = other.vkPhysicalDevice;
		};

		~PhysicalDevice() {};

		const Memory::Type& FindMemoryType(unsigned filter, Memory::PropertyFlag memoryProperties) const;

		operator VkPhysicalDevice() const {
			return vkPhysicalDevice;
		}

	private:
		VkPhysicalDevice vkPhysicalDevice = nullptr;

		PhysicalDevice(const SOV::Instance& Instance, VkPhysicalDevice vkPhysicalDevice);
	};

	class Device {
	public:
		const SOV::PhysicalDevice& PhysicalDevice;

		Device(const Device&) = delete;

		Device& operator =(const Device&) = delete;

		Device(const SOV::PhysicalDevice& PhysicalDevice, const ACTL::Array<Extension>& extensions);

		Device(const SOV::PhysicalDevice& PhysicalDevice, const ACTL::Array<Extension>& extensions, const ACTL::Array<ACTL::Array<float>>& queuePriorities);

		Device(Device&& Other) noexcept : 
			PhysicalDevice(Other.PhysicalDevice),
			Queues(ACTL::move(Other.Queues)) {
			this->~Device();

			vkDevice = Other.vkDevice;

			Other.vkDevice = nullptr;
		}

		~Device();

		operator VkDevice() const {
			return vkDevice;
		}

		operator bool() const {
			return vkDevice;
		}

		const ACTL::Array<ACTL::Array<Queue>>& getQueues() const {
			return Queues;
		}

	private:
		ACTL::Array<ACTL::Array<Queue>> Queues;

		VkDevice vkDevice = nullptr;

		void Init(const ACTL::Array<Extension>& extensions);
	};
}