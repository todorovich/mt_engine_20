// Copyright 2023 Micho Todorovich, all rights reserved.
export module InputDevice;

import std;

using namespace std::literals;

export namespace mt::input::model
{
	enum struct InputDevice : unsigned char
	{
		NO_INPUT_DEVICE = 0x00,

		KEYBOARD = 0x01,
		MOUSE = 0x02,
		GAMEPAD = 0x04,
		FLIGHTSTICK = 0x08,
	};

	constexpr std::wstring_view to_wstring(InputDevice input_device) noexcept
	{
		switch (input_device)
		{
			case InputDevice::KEYBOARD: return L"KEYBOARD"sv;
			case InputDevice::MOUSE: return L"MOUSE"sv;
			case InputDevice::GAMEPAD: return L"GAMEPAD";
			case InputDevice::FLIGHTSTICK: return L"FLIGHT STICK"sv;
			default: return L""sv;
		}
	}
}