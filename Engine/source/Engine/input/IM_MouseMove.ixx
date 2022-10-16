// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module IM_MouseMove;

export import InputMessage;
export import std.core;

export namespace mt::input
{
	class IM_MouseMove : public InputMessage
	{
	public:

		IM_MouseMove(__int32 x = 0, __int32 y = 0)
			: InputMessage(x , y)
		{}

		IM_MouseMove(const IM_MouseMove& other)
			: InputMessage(other)
		{}

		IM_MouseMove(IM_MouseMove&& other)
			: InputMessage(std::move(other))
		{}

		~IM_MouseMove() = default;

		IM_MouseMove& operator=(const IM_MouseMove& other)
		{
			i64 = other.i64;
			
			return *this;
		}

		IM_MouseMove& operator=(IM_MouseMove&& other)
		{
			i64 = std::move(other.i64);

			return *this;
		}
		
		virtual void Execute() 
		{
			InputMessage::_MouseMove(i32[0], i32[1]);
		};

	private:
		__int32& _x = i32[0];
		__int32& _y = i32[1];
	};
}