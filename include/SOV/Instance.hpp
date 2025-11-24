#pragma once

#include "Device.hpp"

namespace SOV {
	class Instance {
	public:
		struct Info {
			Version appVersion;

			ACTL::String<char> appName;

			Version engineVersion;

			ACTL::String<char> engineName;

			Version vulkanVersion;

			ACTL::Array<Layer> layers;

			ACTL::Array<Extension> extensions;
		};

		const Info info;

		Instance& operator =(const Instance&) = delete;

		Instance(const Info& info) : info(info) {
			Init();
		}

		Instance(const Instance& Other) : info(Other.info) {
			Init();
		}

		~Instance();

		operator VkInstance() const {
			return vkInstance;
		}

		operator bool() const {
			return vkInstance;
		}

		const ACTL::Array<PhysicalDevice>& getPhysicalDevices() const {
			return PhysicalDevices;
		}

	private:
		VkInstance vkInstance = nullptr;

		ACTL::Array<PhysicalDevice> PhysicalDevices = 0;

		void Init();

		void GetDevices();
	};
}