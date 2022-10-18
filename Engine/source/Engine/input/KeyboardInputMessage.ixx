// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module KeyboardInputMessage;

export import KeyboardKeys;

export namespace mt::input
{
	class InputManager;

	class KeyboardInputMessage
	{
		InputManager* _input_manager;

	public:

		const KeyboardKeys	key;
		const KeyState		key_state;

		KeyboardInputMessage(
			InputManager* input_manager = nullptr,
			const KeyboardKeys key = KeyboardKeys::NO_KEY,
			const KeyState key_state = KeyState::NO_STATE
		)
			: _input_manager(input_manager)
			, key(key)
			, key_state(key_state)
		{}

		KeyboardInputMessage(const KeyboardInputMessage& other) = default;

		KeyboardInputMessage(KeyboardInputMessage&& other) = default;

		~KeyboardInputMessage() = default;

		KeyboardInputMessage& operator=(const KeyboardInputMessage& other) = default;

		KeyboardInputMessage& operator=(KeyboardInputMessage&& other) = default;
	};
}