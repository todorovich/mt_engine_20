// Copyright 2023 Micho Todorovich, all rights reserved.
export module InputType;

import std;

export import InputContext;
export import InputDataType;
export import InputDevice;
export import VirtualKeyCode;
export import MicrosoftVirtualKeyCode;

using namespace mt::input;

export namespace mt::input::model
{
	struct InputType
	{
		const InputDevice		input_device;
		const InputDataType		input_data_type;
		const InputContext		input_context;
		const VirtualKeyCode	virtual_key_code;

		constexpr InputType(
			const InputDevice input_device_in = InputDevice::NO_INPUT_DEVICE,
			const InputDataType input_data_type_in = InputDataType::NO_DATA_TYPE,
			const InputContext input_context_in = InputContext::NO_CONTEXT,
			const VirtualKeyCode virtual_key_code_in = VirtualKeyCode::NO_KEY
		) noexcept
			: input_device(input_device_in)
			, input_data_type(input_data_type_in)
			, input_context(input_context_in)
			, virtual_key_code(virtual_key_code_in)
		{}

		constexpr InputType(
			const InputDataType input_data_type_in,
			const InputContext input_context_in,
			const MicrosoftVirtualKeyCode virtual_key_code_in
		) noexcept
			: InputType(std::move(getInputType(input_data_type_in, input_context_in, virtual_key_code_in)))
		{}

		static constexpr InputType getInputType(
			const InputDataType input_data_type_in,
			const InputContext input_context_in,
			const MicrosoftVirtualKeyCode virtual_key_code_in
		) noexcept
		{
			switch (virtual_key_code_in)
			{
				case MicrosoftVirtualKeyCode::LEFT_MOUSE_BUTTON:
					return InputType(InputDevice::MOUSE, input_data_type_in, input_context_in, VirtualKeyCode::ONE);
					break;

				case MicrosoftVirtualKeyCode::RIGHT_MOUSE_BUTTON:
					return InputType(InputDevice::MOUSE, input_data_type_in, input_context_in, VirtualKeyCode::TWO);
					break;

				case MicrosoftVirtualKeyCode::MIDDLE_MOUSE_BUTTON:
					return InputType(InputDevice::MOUSE, input_data_type_in, input_context_in, VirtualKeyCode::THREE);
					break;

				case MicrosoftVirtualKeyCode::MOUSE_BUTTON_FOUR:
					return InputType(InputDevice::MOUSE, input_data_type_in, input_context_in, VirtualKeyCode::FOUR);
					break;

				case MicrosoftVirtualKeyCode::MOUSE_BUTTON_FIVE:
					return InputType(InputDevice::MOUSE, input_data_type_in, input_context_in, VirtualKeyCode::FIVE);
					break;

				case MicrosoftVirtualKeyCode::BREAK:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PAUSE);
					break;

				case MicrosoftVirtualKeyCode::SHIFT:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::SHIFT);
					break;

				case MicrosoftVirtualKeyCode::CONTROL:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::CONTROL);
					break;

				case MicrosoftVirtualKeyCode::ALT:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::ALT);
					break;

				case MicrosoftVirtualKeyCode::PAUSE:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PAUSE);
					break;

				case MicrosoftVirtualKeyCode::CAPS_LOCK:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::CAPS_LOCK);
					break;

				case MicrosoftVirtualKeyCode::ESCAPE:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::ESCAPE);
					break;

				case MicrosoftVirtualKeyCode::PRIOR:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PAGE_UP);
					break;

				case MicrosoftVirtualKeyCode::NEXT:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PAGE_DOWN);
					break;

				case MicrosoftVirtualKeyCode::END:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::END);
					break;

				case MicrosoftVirtualKeyCode::HOME:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::HOME);
					break;

				case MicrosoftVirtualKeyCode::LEFT_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::LEFT_ARROW);
					break;

				case MicrosoftVirtualKeyCode::UP_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::UP_ARROW);
					break;

				case MicrosoftVirtualKeyCode::RIGHT_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::RIGHT_ARROW);
					break;

				case MicrosoftVirtualKeyCode::DOWN_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::DOWN_ARROW);
					break;

				case MicrosoftVirtualKeyCode::PRINT_SCREEN:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PRINT_SCREEN);
					break;

				case MicrosoftVirtualKeyCode::INSERT:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::INSERT);
					break;

				case MicrosoftVirtualKeyCode::DEL:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::DEL);
					break;

				case MicrosoftVirtualKeyCode::MENU:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::APPLICATIONS);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_0:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::ZERO);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_1:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::ONE);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_2:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::TWO);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_3:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::THREE);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_4:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::FOUR);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_5:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::FIVE);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_6:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::SIX);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_7:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::SEVEN);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_8:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::EIGHT);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_9:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::NINE);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_MULTIPLY:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::MULTIPLY);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_ADD:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::ADD);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_SUBTRACT:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::SUBTRACT);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_DIVIDE:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::DIVIDE);
					break;

				case MicrosoftVirtualKeyCode::NUMPAD_DECIMAL:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::PERIOD);
					break;

					// FUNCTION KEYS

				case MicrosoftVirtualKeyCode::F1:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::ONE);
					break;

				case MicrosoftVirtualKeyCode::F2:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::TWO);
					break;

				case MicrosoftVirtualKeyCode::F3:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::THREE);
					break;

				case MicrosoftVirtualKeyCode::F4:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::FOUR);
					break;

				case MicrosoftVirtualKeyCode::F5:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::FIVE);
					break;

				case MicrosoftVirtualKeyCode::F6:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::SIX);
					break;

				case MicrosoftVirtualKeyCode::F7:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::SEVEN);
					break;

				case MicrosoftVirtualKeyCode::F8:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::EIGHT);
					break;

				case MicrosoftVirtualKeyCode::F9:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::NINE);
					break;

				case MicrosoftVirtualKeyCode::F10:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::A);
					break;

				case MicrosoftVirtualKeyCode::F11:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::B);
					break;

				case MicrosoftVirtualKeyCode::F12:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::C);
					break;

				case MicrosoftVirtualKeyCode::NUM_LOCK:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::NUM_LOCK);
					break;

				case MicrosoftVirtualKeyCode::SCROLL_LOCK:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::SCROLL_LOCK);
					break;

				case MicrosoftVirtualKeyCode::SEMICOLON:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::SEMICOLON);
					break;

				case MicrosoftVirtualKeyCode::EQUALS:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::EQUALS);
					break;

				case MicrosoftVirtualKeyCode::COMMA:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::COMMA);
					break;

				case MicrosoftVirtualKeyCode::MINUS:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::MINUS);
					break;

				case MicrosoftVirtualKeyCode::PERIOD:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PERIOD);
					break;

				case MicrosoftVirtualKeyCode::FORWARD_SLASH:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::FORWARD_SLASH);
					break;

				case MicrosoftVirtualKeyCode::BACKTICK:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::BACKTICK);
					break;

				case MicrosoftVirtualKeyCode::LEFT_BRACKET:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::LEFT_SQUARE_BRACKET);
					break;

				case MicrosoftVirtualKeyCode::RIGHT_BRACKET:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::RIGHT_SQUARE_BRACKET);
					break;

				case MicrosoftVirtualKeyCode::BACK_SLASH:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::BACK_SLASH);
					break;

				case MicrosoftVirtualKeyCode::APOSTROPHE:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::APOSTROPHE);
					break;

					/*case MicrosoftVirtualKeyCode::ZERO:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::ZERO);
						break;

					case MicrosoftVirtualKeyCode::ONE:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::ONE);
						break;

					case MicrosoftVirtualKeyCode::TWO:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::TWO);
						break;

					case MicrosoftVirtualKeyCode::THREE:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::THREE);
						break;

					case MicrosoftVirtualKeyCode::FOUR:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::FOUR);
						break;

					case MicrosoftVirtualKeyCode::FIVE:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::FIVE);
						break;

					case MicrosoftVirtualKeyCode::SIX:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::SIX);
						break;

					case MicrosoftVirtualKeyCode::SEVEN:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::SEVEN);
						break;

					case MicrosoftVirtualKeyCode::EIGHT:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::EIGHT);
						break;

					case MicrosoftVirtualKeyCode::NINE:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::NINE);
						break;

					case MicrosoftVirtualKeyCode::A:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::A);
						break;

					case MicrosoftVirtualKeyCode::B:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::B);
						break;

					case MicrosoftVirtualKeyCode::C:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::C);
						break;

					case MicrosoftVirtualKeyCode::D:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::D);
						break;

					case MicrosoftVirtualKeyCode::E:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::E);
						break;

					case MicrosoftVirtualKeyCode::F:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::F);
						break;

					case MicrosoftVirtualKeyCode::G:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::G);
						break;

					case MicrosoftVirtualKeyCode::H:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::H);
						break;

					case MicrosoftVirtualKeyCode::I:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::I);
						break;

					case MicrosoftVirtualKeyCode::J:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::J);
						break;

					case MicrosoftVirtualKeyCode::K:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::K);
						break;

					case MicrosoftVirtualKeyCode::L:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::L);
						break;

					case MicrosoftVirtualKeyCode::M:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::M);
						break;

					case MicrosoftVirtualKeyCode::N:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::N);
						break;

					case MicrosoftVirtualKeyCode::O:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::O);
						break;

					case MicrosoftVirtualKeyCode::P:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::P);
						break;

					case MicrosoftVirtualKeyCode::Q:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::Q);
						break;

					case MicrosoftVirtualKeyCode::R:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::R);
						break;

					case MicrosoftVirtualKeyCode::S:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::S);
						break;

					case MicrosoftVirtualKeyCode::T:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::T);
						break;

					case MicrosoftVirtualKeyCode::U:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::U);
						break;

					case MicrosoftVirtualKeyCode::V:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::V);
						break;

					case MicrosoftVirtualKeyCode::W:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::W);
						break;

					case MicrosoftVirtualKeyCode::X:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::X);
						break;

					case MicrosoftVirtualKeyCode::Y:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::Y);
						break;

					case MicrosoftVirtualKeyCode::Z:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::Z);
						break;*/

				default:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, static_cast<VirtualKeyCode>(virtual_key_code_in));
					break;
			}
		}

		auto operator<=>(const InputType&) const noexcept = default;
	};

	std::wstring to_wstring(InputType input_type) noexcept
	{
		auto device = to_wstring(input_type.input_device);
		auto data_type = to_wstring(input_type.input_data_type);
		auto context = to_wstring(input_type.input_context);
		auto virtual_key_code = to_wstring(input_type.virtual_key_code);

		return std::wstring(L"{ ").append(device).append(L", ").append(data_type).append(L", ").append(context).append(L", ").append(virtual_key_code).append(L" }");
	}
}