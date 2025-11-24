#include "Source.hpp"

namespace SOV {
	Instance::~Instance() {
		if (!vkInstance)
			return;

		PhysicalDevices.Delete();

		vkDestroyInstance(vkInstance, nullptr);

		vkInstance = nullptr;
	}

	void Instance::Init() {
		VkApplicationInfo appInfo = {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = info.appName,
			.applicationVersion = VK_MAKE_VERSION(info.appVersion.major, info.appVersion.minor, info.appVersion.patch),
			.pEngineName = info.engineName,
			.engineVersion = VK_MAKE_VERSION(info.engineVersion.major, info.engineVersion.minor, info.engineVersion.patch),
			.apiVersion = VK_MAKE_VERSION(info.vulkanVersion.major, info.vulkanVersion.minor, info.vulkanVersion.patch),
		};

		const unsigned layerCount = (unsigned)info.layers.GetLength();

		const char** layers = new const char* [layerCount];

		for (unsigned i = 0; i < layerCount; i++)
			layers[i] = info.layers[i];

		const unsigned extensionCount = (unsigned)info.extensions.GetLength();

		const char** extensions = new const char* [extensionCount];

		for (unsigned i = 0; i < extensionCount; i++)
			extensions[i] = info.extensions[i];

		VkInstanceCreateInfo vkInfo = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = layerCount,
			.ppEnabledLayerNames = layers,
			.enabledExtensionCount = extensionCount,
			.ppEnabledExtensionNames = extensions
		};

		VkResult result = vkCreateInstance(&vkInfo, nullptr, &vkInstance);

		delete[] layers;

		delete[] extensions;

		if (result)
			throw Exception("Failed to create instance.", this, (Exception::Type)result);

		GetDevices();
	}

	void Instance::GetDevices() {
		unsigned deviceCount;

		vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

		PhysicalDevices = deviceCount;
		
		VkPhysicalDevice* devices = new VkPhysicalDevice[deviceCount];

		vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices);

		for (unsigned i = 0; i < deviceCount; i++)
			PhysicalDevices.EmplaceBack(SOV::PhysicalDevice(*this, devices[i]));

		delete[] devices;
	}
}