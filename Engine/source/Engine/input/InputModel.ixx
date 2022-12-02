// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module InputModel;

export import std.core;

export import Time;

export import MicrosoftVirtualKeyCode;

export import InputDevice;
export import InputDataType;
export import InputContext;
export import VirtualKeyCode;

export using namespace std::literals;

export namespace mt::input
{
	struct InputType
	{
		const InputDevice		input_device;
		const InputDataType		input_data_type;
		const InputContext		input_context;
		const VirtualKeyCode	virtual_key_code;

		constexpr InputType(
			const InputDevice input_device = InputDevice::NO_INPUT_DEVICE,
			const InputDataType input_data_type = InputDataType::NO_DATA_TYPE,
			const InputContext input_context = InputContext::NO_CONTEXT,
			const VirtualKeyCode virtual_key_code = VirtualKeyCode::NO_KEY
		)
			: input_device(input_device)
			, input_data_type(input_data_type)
			, input_context(input_context)
			, virtual_key_code(virtual_key_code)
		{}

		constexpr InputType(
			const InputDataType input_data_type,
			const InputContext input_context_in,
			const MicrosoftVirtualKeyCode virtual_key_code_in
		)
			: InputType(std::move(getInputType(input_data_type, input_context_in, virtual_key_code_in)))
		{}

		constexpr InputType getInputType(
			const InputDataType input_data_type, const InputContext input_context, const MicrosoftVirtualKeyCode virtual_key_code_in
		)
		{
			switch (virtual_key_code_in)
			{
				case mt::input::MicrosoftVirtualKeyCode::LEFT_MOUSE_BUTTON:
					return InputType(InputDevice::MOUSE, input_data_type, input_context, VirtualKeyCode::ONE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::RIGHT_MOUSE_BUTTON:
					return InputType(InputDevice::MOUSE, input_data_type, input_context, VirtualKeyCode::TWO);
					break;

				case mt::input::MicrosoftVirtualKeyCode::MIDDLE_MOUSE_BUTTON:
					return InputType(InputDevice::MOUSE, input_data_type, input_context, VirtualKeyCode::THREE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::MOUSE_BUTTON_FOUR:
					return InputType(InputDevice::MOUSE, input_data_type, input_context, VirtualKeyCode::FOUR);
					break;

				case mt::input::MicrosoftVirtualKeyCode::MOUSE_BUTTON_FIVE:
					return InputType(InputDevice::MOUSE, input_data_type, input_context, VirtualKeyCode::FIVE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::BREAK:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::PAUSE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::SHIFT:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::SHIFT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::CONTROL:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::CONTROL);
					break;

				case mt::input::MicrosoftVirtualKeyCode::ALT:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::ALT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::PAUSE:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::PAUSE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::CAPS_LOCK:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::CAPS_LOCK);
					break;

				case mt::input::MicrosoftVirtualKeyCode::ESCAPE:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::ESCAPE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::PRIOR:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::PAGE_UP);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NEXT:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::PAGE_DOWN);
					break;

				case mt::input::MicrosoftVirtualKeyCode::END:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::END);
					break;

				case mt::input::MicrosoftVirtualKeyCode::HOME:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::HOME);
					break;

				case mt::input::MicrosoftVirtualKeyCode::LEFT_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::LEFT_ARROW);
					break;

				case mt::input::MicrosoftVirtualKeyCode::UP_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::UP_ARROW);
					break;

				case mt::input::MicrosoftVirtualKeyCode::RIGHT_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::RIGHT_ARROW);
					break;

				case mt::input::MicrosoftVirtualKeyCode::DOWN_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::DOWN_ARROW);
					break;

				case mt::input::MicrosoftVirtualKeyCode::PRINT_SCREEN:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::PRINT_SCREEN);
					break;

				case mt::input::MicrosoftVirtualKeyCode::INSERT:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::INSERT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::DEL:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::DEL);
					break;

				case mt::input::MicrosoftVirtualKeyCode::MENU:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::APPLICATIONS);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_0:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::ZERO);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_1:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::ONE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_2:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::TWO);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_3:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::THREE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_4:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::FOUR);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_5:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::FIVE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_6:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::SIX);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_7:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::SEVEN);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_8:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::EIGHT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_9:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::NINE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_MULTIPLY:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::MULTIPLY);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_ADD:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::ADD);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_SUBTRACT:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::SUBTRACT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_DIVIDE:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::DIVIDE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_DECIMAL:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::EXTENDED_KEY | input_context, VirtualKeyCode::PERIOD);
					break;

					// FUNCTION KEYS

				case mt::input::MicrosoftVirtualKeyCode::F1:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::ONE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F2:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::TWO);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F3:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::THREE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F4:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::FOUR);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F5:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::FIVE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F6:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::SIX);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F7:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::SEVEN);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F8:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::EIGHT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F9:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::NINE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F10:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::A);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F11:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::B);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F12:
					return InputType(InputDevice::KEYBOARD, input_data_type, InputContext::FUNCTION_KEY | input_context, VirtualKeyCode::C);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUM_LOCK:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::NUM_LOCK);
					break;

				case mt::input::MicrosoftVirtualKeyCode::SCROLL_LOCK:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::SCROLL_LOCK);
					break;

				case mt::input::MicrosoftVirtualKeyCode::SEMICOLON:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::SEMICOLON);
					break;

				case mt::input::MicrosoftVirtualKeyCode::EQUALS:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::EQUALS);
					break;

				case mt::input::MicrosoftVirtualKeyCode::COMMA:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::COMMA);
					break;

				case mt::input::MicrosoftVirtualKeyCode::MINUS:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::MINUS);
					break;

				case mt::input::MicrosoftVirtualKeyCode::PERIOD:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::PERIOD);
					break;

				case mt::input::MicrosoftVirtualKeyCode::FORWARD_SLASH:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::FORWARD_SLASH);
					break;

				case mt::input::MicrosoftVirtualKeyCode::BACKTICK:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::BACKTICK);
					break;

				case mt::input::MicrosoftVirtualKeyCode::LEFT_BRACKET:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::LEFT_SQUARE_BRACKET);
					break;

				case mt::input::MicrosoftVirtualKeyCode::RIGHT_BRACKET:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::RIGHT_SQUARE_BRACKET);
					break;

				case mt::input::MicrosoftVirtualKeyCode::BACK_SLASH:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::BACK_SLASH);
					break;

				case mt::input::MicrosoftVirtualKeyCode::APOSTROPHE:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, VirtualKeyCode::APOSTROPHE);
					break;

					/*case mt::input::MicrosoftVirtualKeyCode::ZERO:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::ZERO);
						break;

					case mt::input::MicrosoftVirtualKeyCode::ONE:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::ONE);
						break;

					case mt::input::MicrosoftVirtualKeyCode::TWO:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::TWO);
						break;

					case mt::input::MicrosoftVirtualKeyCode::THREE:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::THREE);
						break;

					case mt::input::MicrosoftVirtualKeyCode::FOUR:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::FOUR);
						break;

					case mt::input::MicrosoftVirtualKeyCode::FIVE:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::FIVE);
						break;

					case mt::input::MicrosoftVirtualKeyCode::SIX:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::SIX);
						break;

					case mt::input::MicrosoftVirtualKeyCode::SEVEN:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::SEVEN);
						break;

					case mt::input::MicrosoftVirtualKeyCode::EIGHT:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::EIGHT);
						break;

					case mt::input::MicrosoftVirtualKeyCode::NINE:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::NINE);
						break;

					case mt::input::MicrosoftVirtualKeyCode::A:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::A);
						break;

					case mt::input::MicrosoftVirtualKeyCode::B:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::B);
						break;

					case mt::input::MicrosoftVirtualKeyCode::C:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::C);
						break;

					case mt::input::MicrosoftVirtualKeyCode::D:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::D);
						break;

					case mt::input::MicrosoftVirtualKeyCode::E:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::E);
						break;

					case mt::input::MicrosoftVirtualKeyCode::F:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::F);
						break;

					case mt::input::MicrosoftVirtualKeyCode::G:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::G);
						break;

					case mt::input::MicrosoftVirtualKeyCode::H:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::H);
						break;

					case mt::input::MicrosoftVirtualKeyCode::I:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::I);
						break;

					case mt::input::MicrosoftVirtualKeyCode::J:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::J);
						break;

					case mt::input::MicrosoftVirtualKeyCode::K:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::K);
						break;

					case mt::input::MicrosoftVirtualKeyCode::L:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::L);
						break;

					case mt::input::MicrosoftVirtualKeyCode::M:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::M);
						break;

					case mt::input::MicrosoftVirtualKeyCode::N:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::N);
						break;

					case mt::input::MicrosoftVirtualKeyCode::O:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::O);
						break;

					case mt::input::MicrosoftVirtualKeyCode::P:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::P);
						break;

					case mt::input::MicrosoftVirtualKeyCode::Q:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::Q);
						break;

					case mt::input::MicrosoftVirtualKeyCode::R:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::R);
						break;

					case mt::input::MicrosoftVirtualKeyCode::S:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::S);
						break;

					case mt::input::MicrosoftVirtualKeyCode::T:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::T);
						break;

					case mt::input::MicrosoftVirtualKeyCode::U:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::U);
						break;

					case mt::input::MicrosoftVirtualKeyCode::V:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::V);
						break;

					case mt::input::MicrosoftVirtualKeyCode::W:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::W);
						break;

					case mt::input::MicrosoftVirtualKeyCode::X:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::X);
						break;

					case mt::input::MicrosoftVirtualKeyCode::Y:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::Y);
						break;

					case mt::input::MicrosoftVirtualKeyCode::Z:
						return InputType(InputDevice::KEYBOARD , input_data_type , input_context, VirtualKeyCode::Z);
						break;*/

				default:
					return InputType(InputDevice::KEYBOARD, input_data_type, input_context, static_cast<VirtualKeyCode>(virtual_key_code));
					break;
			}

			return InputType();
		}

		auto operator<=>(const InputType&) const = default;
	};

	struct InputData2D
	{
		const int x;
		const int y;

		InputData2D(const int x, const int y)
			: x(x)
			, y(y)
		{}
	};

	struct InputData3D
	{
		const int x;
		const int y;
		const int z;

		InputData3D(const int x = 0, const int y = 0, const int z = 0)
			: x(x)
			, y(y)
			, z(z)
		{}
	};

	// TODO: add 1-3d input as appropriate
	struct InputMessage
	{
		const InputType				input_type;
		const mt::time::TimePoint	time_point;

		const std::variant<std::monostate, int, InputData2D, InputData3D> data;

		InputMessage(
			const InputType input_type = InputType(),
			const mt::time::TimePoint time_point = mt::time::Clock::now(),
			const std::variant<std::monostate, int, InputData2D, InputData3D> data = std::monostate()
		)
			: input_type(input_type)
			, time_point(time_point)
			, data(data)
		{

		}
	};
}