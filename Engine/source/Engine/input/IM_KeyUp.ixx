// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module IM_KeyUp;

export import InputMessage;
export import std.core;

export namespace mt::input
{
	class IM_KeyUp : public InputMessage
	{
	public:

		IM_KeyUp(WPARAM button_up)
			: InputMessage(static_cast<__int64>(button_up))
		{}

		IM_KeyUp(const IM_KeyUp& other)
			: InputMessage(other)
		{}

		IM_KeyUp(IM_KeyUp&& other)
			: InputMessage(std::move(other))
		{}

		~IM_KeyUp() = default;

		IM_KeyUp& operator=(const IM_KeyUp& other)
		{
			i64 = other.i64;

			return *this;
		}

		IM_KeyUp& operator=(IM_KeyUp&& other)
		{
			i64 = std::move(other.i64);

			return *this;
		}

		virtual void Execute()
		{
			_KeyUp(_button_up);
		};

	private:
		WPARAM & _button_up = *reinterpret_cast<WPARAM*>(&i64);
	};
}