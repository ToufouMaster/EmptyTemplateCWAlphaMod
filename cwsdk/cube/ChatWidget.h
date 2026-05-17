#pragma once

#include "../non_cube.h"
#include "../plasma.h"
#include "../msvc_bincompat.h"

namespace cube {
	struct ChatWidget : plasma::Widget
	{
		//uint32_t field_0x15c;
		int field_160;
		int field_164;
		MSVCBinCompat::wstring current_typed_message;
		uint8_t is_typing_message;
		int field_184;
		int field_188;
		int field_18C;

		void Print(std::wstring str, Color color);
	};
};