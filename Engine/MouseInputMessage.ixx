// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module MouseInputMessage;

export import std.core;

export namespace mt::input
{
	class InputManager;

	enum struct MouseButtons {
		LEFT	= 0x01,
		RIGHT	= 0x02,
		MIDDLE	= 0x04,    
		FOUR    = 0x05,    
		FIVE	= 0x06,     
	};

	class MouseInputMessage
	{
		InputManager* _input_manager;

	public:
		const __int32 x;
		const __int32 y;
		const bool left_button;
		const bool right_button;
		const bool middle_button;
		const bool button_4;
		const bool button_5;

		MouseInputMessage(
			InputManager* input_manager = nullptr, __int32 x = 0, __int32 y= 0, 
			bool _left_button = false, bool _right_button = false, bool _middle_button = false, 
			bool _button_4 = false, bool _button_5 = false
		)
			: _input_manager(input_manager)
			, x(x)
			, y(y)
			, left_button(_left_button)
			, right_button(_right_button)
			, middle_button(_middle_button)
			, button_4(_button_4)
			, button_5(_button_5)
		{}

		MouseInputMessage(const MouseInputMessage& other) = default;
		
		MouseInputMessage(MouseInputMessage&& other) = default;

		~MouseInputMessage() = default;

		MouseInputMessage& operator=(const MouseInputMessage& other) = default;

		MouseInputMessage& operator=(MouseInputMessage&& other) = default;
	};
}