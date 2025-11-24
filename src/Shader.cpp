#include "Source.hpp"

namespace SOV {
	namespace Shader {
		Module::Module(const SOV::Device& Device, ACTL::Array<unsigned> code, const Info& info) : Device(Device), info(info) {
			VkShaderModuleCreateInfo vkInfo = {
				.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				.codeSize = (unsigned)code.GetLength() * 4,
				.pCode = code.begin(),
			};

			VkResult result = vkCreateShaderModule(Device, &vkInfo, nullptr, &vkModule);

			if (result)
				throw Exception("Failed to create shader module.", this, (Exception::Type)result);
		}

		Module::~Module() {
			if (!vkModule)
				return;

			vkDestroyShaderModule(Device, vkModule, nullptr);

			vkModule = nullptr;
		}
	}
}