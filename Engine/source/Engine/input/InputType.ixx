export module InputType;

export import <compare>;
export import <string>;

export import InputContext;
export import InputDataType;
export import InputDevice;
export import VirtualKeyCode;
export import MicrosoftVirtualKeyCode;

export namespace mt::input
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
		)
			: input_device(input_device_in)
			, input_data_type(input_data_type_in)
			, input_context(input_context_in)
			, virtual_key_code(virtual_key_code_in)
		{}

		constexpr InputType(
			const InputDataType input_data_type_in,
			const InputContext input_context_in,
			const MicrosoftVirtualKeyCode virtual_key_code_in
		)
			: InputType(std::move(getInputType(input_data_type_in, input_context_in, virtual_key_code_in)))
		{}

		constexpr InputType getInputType(
			const InputDataType input_data_type_in, const InputContext input_context_in, const MicrosoftVirtualKeyCode virtual_key_code_in
		)
		{
			switch (virtual_key_code_in)
			{
				case mt::input::MicrosoftVirtualKeyCode::LEFT_MOUSE_BUTTON:
					return InputType(InputDevice::MOUSE, input_data_type_in, input_context_in, VirtualKeyCode::ONE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::RIGHT_MOUSE_BUTTON:
					return InputType(InputDevice::MOUSE, input_data_type_in, input_context_in, VirtualKeyCode::TWO);
					break;

				case mt::input::MicrosoftVirtualKeyCode::MIDDLE_MOUSE_BUTTON:
					return InputType(InputDevice::MOUSE, input_data_type_in, input_context_in, VirtualKeyCode::THREE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::MOUSE_BUTTON_FOUR:
					return InputType(InputDevice::MOUSE, input_data_type_in, input_context_in, VirtualKeyCode::FOUR);
					break;

				case mt::input::MicrosoftVirtualKeyCode::MOUSE_BUTTON_FIVE:
					return InputType(InputDevice::MOUSE, input_data_type_in, input_context_in, VirtualKeyCode::FIVE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::BREAK:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PAUSE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::SHIFT:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::SHIFT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::CONTROL:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::CONTROL);
					break;

				case mt::input::MicrosoftVirtualKeyCode::ALT:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::ALT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::PAUSE:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PAUSE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::CAPS_LOCK:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::CAPS_LOCK);
					break;

				case mt::input::MicrosoftVirtualKeyCode::ESCAPE:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::ESCAPE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::PRIOR:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PAGE_UP);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NEXT:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PAGE_DOWN);
					break;

				case mt::input::MicrosoftVirtualKeyCode::END:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::END);
					break;

				case mt::input::MicrosoftVirtualKeyCode::HOME:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::HOME);
					break;

				case mt::input::MicrosoftVirtualKeyCode::LEFT_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::LEFT_ARROW);
					break;

				case mt::input::MicrosoftVirtualKeyCode::UP_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::UP_ARROW);
					break;

				case mt::input::MicrosoftVirtualKeyCode::RIGHT_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::RIGHT_ARROW);
					break;

				case mt::input::MicrosoftVirtualKeyCode::DOWN_ARROW:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::DOWN_ARROW);
					break;

				case mt::input::MicrosoftVirtualKeyCode::PRINT_SCREEN:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PRINT_SCREEN);
					break;

				case mt::input::MicrosoftVirtualKeyCode::INSERT:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::INSERT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::DEL:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::DEL);
					break;

				case mt::input::MicrosoftVirtualKeyCode::MENU:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::APPLICATIONS);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_0:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::ZERO);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_1:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::ONE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_2:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::TWO);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_3:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::THREE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_4:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::FOUR);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_5:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::FIVE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_6:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::SIX);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_7:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::SEVEN);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_8:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::EIGHT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_9:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::NINE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_MULTIPLY:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::MULTIPLY);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_ADD:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::ADD);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_SUBTRACT:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::SUBTRACT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_DIVIDE:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::DIVIDE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUMPAD_DECIMAL:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::EXTENDED_KEY | input_context_in, VirtualKeyCode::PERIOD);
					break;

					// FUNCTION KEYS

				case mt::input::MicrosoftVirtualKeyCode::F1:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::ONE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F2:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::TWO);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F3:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::THREE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F4:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::FOUR);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F5:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::FIVE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F6:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::SIX);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F7:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::SEVEN);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F8:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::EIGHT);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F9:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::NINE);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F10:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::A);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F11:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::B);
					break;

				case mt::input::MicrosoftVirtualKeyCode::F12:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, InputContext::FUNCTION_KEY | input_context_in, VirtualKeyCode::C);
					break;

				case mt::input::MicrosoftVirtualKeyCode::NUM_LOCK:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::NUM_LOCK);
					break;

				case mt::input::MicrosoftVirtualKeyCode::SCROLL_LOCK:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::SCROLL_LOCK);
					break;

				case mt::input::MicrosoftVirtualKeyCode::SEMICOLON:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::SEMICOLON);
					break;

				case mt::input::MicrosoftVirtualKeyCode::EQUALS:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::EQUALS);
					break;

				case mt::input::MicrosoftVirtualKeyCode::COMMA:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::COMMA);
					break;

				case mt::input::MicrosoftVirtualKeyCode::MINUS:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::MINUS);
					break;

				case mt::input::MicrosoftVirtualKeyCode::PERIOD:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::PERIOD);
					break;

				case mt::input::MicrosoftVirtualKeyCode::FORWARD_SLASH:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::FORWARD_SLASH);
					break;

				case mt::input::MicrosoftVirtualKeyCode::BACKTICK:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::BACKTICK);
					break;

				case mt::input::MicrosoftVirtualKeyCode::LEFT_BRACKET:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::LEFT_SQUARE_BRACKET);
					break;

				case mt::input::MicrosoftVirtualKeyCode::RIGHT_BRACKET:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::RIGHT_SQUARE_BRACKET);
					break;

				case mt::input::MicrosoftVirtualKeyCode::BACK_SLASH:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::BACK_SLASH);
					break;

				case mt::input::MicrosoftVirtualKeyCode::APOSTROPHE:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, VirtualKeyCode::APOSTROPHE);
					break;

					/*case mt::input::MicrosoftVirtualKeyCode::ZERO:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::ZERO);
						break;

					case mt::input::MicrosoftVirtualKeyCode::ONE:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::ONE);
						break;

					case mt::input::MicrosoftVirtualKeyCode::TWO:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::TWO);
						break;

					case mt::input::MicrosoftVirtualKeyCode::THREE:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::THREE);
						break;

					case mt::input::MicrosoftVirtualKeyCode::FOUR:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::FOUR);
						break;

					case mt::input::MicrosoftVirtualKeyCode::FIVE:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::FIVE);
						break;

					case mt::input::MicrosoftVirtualKeyCode::SIX:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::SIX);
						break;

					case mt::input::MicrosoftVirtualKeyCode::SEVEN:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::SEVEN);
						break;

					case mt::input::MicrosoftVirtualKeyCode::EIGHT:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::EIGHT);
						break;

					case mt::input::MicrosoftVirtualKeyCode::NINE:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::NINE);
						break;

					case mt::input::MicrosoftVirtualKeyCode::A:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::A);
						break;

					case mt::input::MicrosoftVirtualKeyCode::B:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::B);
						break;

					case mt::input::MicrosoftVirtualKeyCode::C:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::C);
						break;

					case mt::input::MicrosoftVirtualKeyCode::D:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::D);
						break;

					case mt::input::MicrosoftVirtualKeyCode::E:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::E);
						break;

					case mt::input::MicrosoftVirtualKeyCode::F:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::F);
						break;

					case mt::input::MicrosoftVirtualKeyCode::G:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::G);
						break;

					case mt::input::MicrosoftVirtualKeyCode::H:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::H);
						break;

					case mt::input::MicrosoftVirtualKeyCode::I:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::I);
						break;

					case mt::input::MicrosoftVirtualKeyCode::J:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::J);
						break;

					case mt::input::MicrosoftVirtualKeyCode::K:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::K);
						break;

					case mt::input::MicrosoftVirtualKeyCode::L:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::L);
						break;

					case mt::input::MicrosoftVirtualKeyCode::M:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::M);
						break;

					case mt::input::MicrosoftVirtualKeyCode::N:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::N);
						break;

					case mt::input::MicrosoftVirtualKeyCode::O:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::O);
						break;

					case mt::input::MicrosoftVirtualKeyCode::P:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::P);
						break;

					case mt::input::MicrosoftVirtualKeyCode::Q:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::Q);
						break;

					case mt::input::MicrosoftVirtualKeyCode::R:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::R);
						break;

					case mt::input::MicrosoftVirtualKeyCode::S:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::S);
						break;

					case mt::input::MicrosoftVirtualKeyCode::T:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::T);
						break;

					case mt::input::MicrosoftVirtualKeyCode::U:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::U);
						break;

					case mt::input::MicrosoftVirtualKeyCode::V:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::V);
						break;

					case mt::input::MicrosoftVirtualKeyCode::W:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::W);
						break;

					case mt::input::MicrosoftVirtualKeyCode::X:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::X);
						break;

					case mt::input::MicrosoftVirtualKeyCode::Y:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::Y);
						break;

					case mt::input::MicrosoftVirtualKeyCode::Z:
						return InputType(InputDevice::KEYBOARD , input_data_type_in , input_context_in, VirtualKeyCode::Z);
						break;*/

				default:
					return InputType(InputDevice::KEYBOARD, input_data_type_in, input_context_in, static_cast<VirtualKeyCode>(virtual_key_code_in));
					break;
			}
		}

		auto operator<=>(const InputType&) const = default;
	};

	std::wstring to_wstring(InputType input_type)
	{
		auto device = to_wstring(input_type.input_device);
		auto data_type = to_wstring(input_type.input_data_type);
		auto context = to_wstring(input_type.input_context);
		auto virtual_key_code = to_wstring(input_type.virtual_key_code);

		return std::wstring(L"{ ").append(device).append(L", ").append(data_type).append(L", ").append(context).append(L", ").append(virtual_key_code).append(L" }");
	}
}