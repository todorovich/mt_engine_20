// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module IM_KeyDown;

export import InputMessage;
export import std.core;

export namespace mt::input
{
	class IM_KeyDown : public InputMessage
	{
	public:

		IM_KeyDown(WPARAM button_down)
			: InputMessage(static_cast<__int64>(button_down))
		{}

		IM_KeyDown(const IM_KeyDown& other)
			: InputMessage(other)
		{}

		IM_KeyDown(IM_KeyDown&& other)
			: InputMessage(std::move(other))
		{}

		~IM_KeyDown() = default;

		IM_KeyDown& operator=(const IM_KeyDown& other)
		{
			i64 = other.i64;

			return *this;
		}

		IM_KeyDown& operator=(IM_KeyDown&& other)
		{
			i64 = std::move(other.i64);

			return *this;
		}

		virtual void Execute()
		{
			_KeyDown(_button_down);
		};

	private:
		WPARAM & _button_down = *reinterpret_cast<WPARAM*>(&i64);
	};
}