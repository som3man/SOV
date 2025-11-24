#pragma once

#include "../ACTL/ACTL.hpp"

#include <vulkan/vulkan.h>

namespace SOV {
	using size = ACTL::size;

	using Flags = VkFlags;
	
	enum class Format {
		UNDEFINED           = VK_FORMAT_UNDEFINED,
		R8_UNORM            = VK_FORMAT_R8_UNORM,
		R8_SNORM            = VK_FORMAT_R8_SNORM,
		R8_UINT             = VK_FORMAT_R8_UINT,
		R8_SINT             = VK_FORMAT_R8_SINT,
		R8_SRGB             = VK_FORMAT_R8_SRGB,
		R8G8_UNORM          = VK_FORMAT_R8G8_UNORM,
		R8G8_SNORM          = VK_FORMAT_R8G8_SNORM,
		R8G8_UINT           = VK_FORMAT_R8G8_UINT,
		R8G8_SINT           = VK_FORMAT_R8G8_SINT,
		R8G8_SRGB           = VK_FORMAT_R8G8_SRGB,
		R8G8B8_UNORM        = VK_FORMAT_R8G8B8_UNORM,
		R8G8B8_SNORM        = VK_FORMAT_R8G8B8_SNORM,
		R8G8B8_UINT         = VK_FORMAT_R8G8B8_UINT,
		R8G8B8_SINT         = VK_FORMAT_R8G8B8_SINT,
		R8G8B8_SRGB         = VK_FORMAT_R8G8B8_SRGB,
		R8G8B8A8_UNORM      = VK_FORMAT_R8G8B8A8_UNORM,
		R8G8B8A8_SNORM      = VK_FORMAT_R8G8B8A8_SNORM,
		R8G8B8A8_UINT       = VK_FORMAT_R8G8B8A8_UINT,
		R8G8B8A8_SINT       = VK_FORMAT_R8G8B8A8_SINT,
		R8G8B8A8_SRGB       = VK_FORMAT_R8G8B8A8_SRGB,
		R32_UINT            = VK_FORMAT_R32_UINT,
		R32_SINT            = VK_FORMAT_R32_SINT,
		R32_SFLOAT          = VK_FORMAT_R32_SFLOAT,
		R32G32_UINT         = VK_FORMAT_R32G32_UINT,
		R32G32_SINT         = VK_FORMAT_R32G32_SINT,
		R32G32_SFLOAT       = VK_FORMAT_R32G32_SFLOAT,
		R32G32B32_UINT      = VK_FORMAT_R32G32B32_UINT,
		R32G32B32_SINT      = VK_FORMAT_R32G32B32_SINT,
		R32G32B32_SFLOAT    = VK_FORMAT_R32G32B32_SFLOAT,
		R32G32B32A32_UINT   = VK_FORMAT_R32G32B32A32_UINT,
		R32G32B32A32_SINT   = VK_FORMAT_R32G32B32A32_SINT,
		R32G32B32A32_SFLOAT = VK_FORMAT_R32G32B32A32_SFLOAT,
	};

	enum SampleCountFlag {
		COUNT_1_BIT  = VK_SAMPLE_COUNT_1_BIT,
		COUNT_2_BIT  = VK_SAMPLE_COUNT_2_BIT,
		COUNT_4_BIT  = VK_SAMPLE_COUNT_4_BIT,
		COUNT_8_BIT  = VK_SAMPLE_COUNT_8_BIT,
		COUNT_16_BIT = VK_SAMPLE_COUNT_16_BIT,
		COUNT_32_BIT = VK_SAMPLE_COUNT_32_BIT,
		COUNT_64_BIT = VK_SAMPLE_COUNT_64_BIT,
	};

	enum class SharingMode {
		EXCLUSIVE  = VK_SHARING_MODE_EXCLUSIVE,
		CONCURRENT = VK_SHARING_MODE_CONCURRENT,
	};

	enum class Filter {
		NEAREST = VK_FILTER_NEAREST,
		LINEAR  = VK_FILTER_LINEAR,
	};

	enum class CompareOp {
		NEVER            = VK_COMPARE_OP_NEVER,
		LESS             = VK_COMPARE_OP_LESS,
		EQUAL            = VK_COMPARE_OP_EQUAL,
		LESS_OR_EQUAL    = VK_COMPARE_OP_LESS_OR_EQUAL,
		GREATER          = VK_COMPARE_OP_GREATER,
		NOT_EQUAL        = VK_COMPARE_OP_NOT_EQUAL,
		GREATER_OR_EQUAL = VK_COMPARE_OP_GREATER_OR_EQUAL,
		ALWAYS           = VK_COMPARE_OP_ALWAYS,
	};

	enum class BorderColor {
		FLOAT_TRANSPARENT_BLACK = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
		INT_TRANSPARENT_BLACK   = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK,
		FLOAT_OPAQUE_BLACK      = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
		INT_OPAQUE_BLACK        = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		FLOAT_OPAQUE_WHITE      = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
		INT_OPAQUE_WHITE        = VK_BORDER_COLOR_INT_OPAQUE_WHITE,
	};

	enum AccessFlag {
		INDIRECT_COMMAND_READ          = VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
		INDEX_READ                     = VK_ACCESS_INDEX_READ_BIT,
		VERTEX_ATTRIBUTE_READ          = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
		UNIFORM_READ                   = VK_ACCESS_UNIFORM_READ_BIT,
		INPUT_ATTACHMENT_READ          = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
		SHADER_READ                    = VK_ACCESS_SHADER_READ_BIT,
		SHADER_WRITE                   = VK_ACCESS_SHADER_WRITE_BIT,
		COLOR_ATTACHMENT_READ          = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
		COLOR_ATTACHMENT_WRITE         = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		DEPTH_STENCIL_ATTACHMENT_READ  = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
		DEPTH_STENCIL_ATTACHMENT_WRITE = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		TRANSFER_READ                  = VK_ACCESS_TRANSFER_READ_BIT,
		TRANSFER_WRITE                 = VK_ACCESS_TRANSFER_WRITE_BIT,
		HOST_READ                      = VK_ACCESS_HOST_READ_BIT,
		HOST_WRITE                     = VK_ACCESS_HOST_WRITE_BIT,
		MEMORY_READ                    = VK_ACCESS_MEMORY_READ_BIT,
		MEMORY_WRITE                   = VK_ACCESS_MEMORY_WRITE_BIT,
	};

	enum DependencyFlag {
		BY_REGION    = VK_DEPENDENCY_BY_REGION_BIT,
		DEVICE_GROUP = VK_DEPENDENCY_DEVICE_GROUP_BIT,
		VIEW_LOCAL   = VK_DEPENDENCY_VIEW_LOCAL_BIT,
	};

	struct ComponentMapping {
		enum class Swizzle {
			IDENTITY = VK_COMPONENT_SWIZZLE_IDENTITY,
			ZERO     = VK_COMPONENT_SWIZZLE_ZERO,
			ONE      = VK_COMPONENT_SWIZZLE_ONE,
			RED      = VK_COMPONENT_SWIZZLE_R,
			GREEN    = VK_COMPONENT_SWIZZLE_G,
			BLUE     = VK_COMPONENT_SWIZZLE_B,
			ALPHA    = VK_COMPONENT_SWIZZLE_A,
		};

		Swizzle r, g, b, a;

		operator VkComponentMapping() const {
			return {
				.r = (VkComponentSwizzle)r,
				.g = (VkComponentSwizzle)g,
				.b = (VkComponentSwizzle)b,
				.a = (VkComponentSwizzle)a,
			};
		}
	};

	class Exception {
	public:
		enum Type {
			OTHER                 = -1,
			UNDEFINED             = 0,
			OUT_OF_HOST_MEMORY    = VK_ERROR_OUT_OF_HOST_MEMORY,
			OUT_OF_DEVICE_MEMORY  = VK_ERROR_OUT_OF_DEVICE_MEMORY,
			INITIALIZATION_FAILED = VK_ERROR_INITIALIZATION_FAILED,
			DEVICE_LOST           = VK_ERROR_DEVICE_LOST,
			MEMORY_MAP_FAILED     = VK_ERROR_MEMORY_MAP_FAILED,
			LAYER_NOT_PRESENT     = VK_ERROR_LAYER_NOT_PRESENT,
			EXTENSION_NOT_PRESENT = VK_ERROR_EXTENSION_NOT_PRESENT,
			FEATURE_NOT_PRESENT   = VK_ERROR_FEATURE_NOT_PRESENT,
			INCOMPATIBLE_DRIVER   = VK_ERROR_INCOMPATIBLE_DRIVER,
			TOO_MANY_OBJECTS      = VK_ERROR_TOO_MANY_OBJECTS,
			FORMAT_NOT_SUPPORTED  = VK_ERROR_FORMAT_NOT_SUPPORTED,
			FRAGMENTED_POOL       = VK_ERROR_FRAGMENTED_POOL,
		};

		const ACTL::String<char> text;

		const void* const address;

		const Type type;

		Exception(const char* text, const void* address, Type type) : text(text), address(address), type(type) {};

		~Exception() {};
	};

	struct Version {
		unsigned short major, minor, patch;
	};

	using Layer = ACTL::String<char>;

	using Extension = ACTL::String<char>;
	
	struct Extent2 {
		unsigned width, height;

		operator VkExtent2D() const {
			return {
				.width = width,
				.height = height
			};
		}
	};

	struct Extent3 {
		unsigned width, height, depth;

		operator VkExtent3D() const {
			return {
				.width = width,
				.height = height,
				.depth = depth
			};
		}
	};

	struct Offset2 {
		int x, y;

		operator VkOffset2D() const {
			return {
				.x = x,
				.y = y
			};
		}
	};

	struct Offset3 {
		int x, y, z;

		operator VkOffset3D() const {
			return {
				.x = x,
				.y = y,
				.z = z
			};
		}
	};

	struct Rect2 {
		Offset2 offset;

		Extent2 extent;

		operator VkRect2D() const {
			return {
				.offset = (VkOffset2D)offset,
				.extent = (VkExtent2D)extent
			};
		}
	};
}