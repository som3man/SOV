#pragma once

#include "Device.hpp"

namespace SOV {
	namespace Shader {
		enum StageFlag {
			VERTEX                  = VK_SHADER_STAGE_VERTEX_BIT,
			TESSELLATION_CONTROL    = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
			TESSELLATION_EVALUATION = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
			GEOMETRY                = VK_SHADER_STAGE_GEOMETRY_BIT,
			FRAGMENT                = VK_SHADER_STAGE_FRAGMENT_BIT,
			COMPUTE                 = VK_SHADER_STAGE_COMPUTE_BIT,
			ALL_GRAPHICS            = VK_SHADER_STAGE_ALL_GRAPHICS,
			ALL                     = VK_SHADER_STAGE_ALL,
		};

		class Module {
		public:
			struct Info {
				StageFlag stageFlags;

				ACTL::String<char> entryPointName;
			};

			const SOV::Device& Device;

			Module(const Module&) = delete;

			Module& operator =(const Module&) = delete;

			Module(const SOV::Device& Device, ACTL::Array<unsigned> code, const Info& info);

			Module(Module&& Other) noexcept : Device(Other.Device), info(ACTL::move(Other.info)) {
				vkModule = Other.vkModule;

				Other.vkModule = nullptr;
			}

			~Module();

			operator VkShaderModule() const {
				return vkModule;
			}

			operator bool() const {
				return vkModule;
			}

			const Info& getInfo() const {
				return info;
			}

		private:
			VkShaderModule vkModule = nullptr;

			Info info;
		};
	}
}