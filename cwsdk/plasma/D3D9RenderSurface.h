#pragma once

#include <cstdint>

namespace plasma {
	class Node;

	class D3D9RenderSurface {
	public:
		void* vftable;
		uint32_t field_04;
		float red_filter_multiplier;
		float green_filter_multiplier;
		float blue_filter_multiplier;
		float alpha_filter_multiplier;
		float unknown_filter_multiplier1;
		float unknown_filter_multiplier2;
		float unknown_filter_multiplier3;
		float unknown_filter_multiplier4;
		uint32_t field_28;
		int32_t pixel_width;
		int32_t pixel_height;
		void* D3D9Engine;
		void* rendering_properties;
	};
};