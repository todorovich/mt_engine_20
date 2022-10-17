// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module IM_MouseUp;

export import InputMessage;
export import std.core;

export namespace mt::input
{
	class InputManager;

	class IM_MouseUp : public InputMessage
	{
	public:

		IM_MouseUp(InputManager* input_manager, WPARAM button_down)
			: InputMessage(input_manager, static_cast<__int64>(button_down))
		{}

		IM_MouseUp(const IM_MouseUp& other)
			: InputMessage(other)
		{}

		IM_MouseUp(IM_MouseUp&& other)
			: InputMessage(std::move(other))
		{}

		~IM_MouseUp() = default;

		IM_MouseUp& operator=(const IM_MouseUp& other)
		{
			i64 = other.i64;

			return *this;
		}

		IM_MouseUp& operator=(IM_MouseUp&& other)
		{
			i64 = std::move(other.i64);

			return *this;
		}

		virtual void Execute()
		{
			_MouseUp(_button_up);
		};

	private:
		WPARAM & _button_up = *reinterpret_cast<WPARAM*>(&i64);
	};
}