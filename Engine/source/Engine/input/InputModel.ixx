// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module InputModel;

#pragma warning( push )
#pragma warning( disable : 5050 )
export import std.core;
#pragma warning( pop )

export import Time;

export import MicrosoftVirtualKeyCode;

export import InputDevice;
export import InputDataType;
export import InputContext;
export import VirtualKeyCode;
export import InputType;

export using namespace std::literals;

export namespace mt { class Engine; }

export namespace mt::input
{
	using button_function = void(mt::Engine&);
	using one_dimensional_function = void(mt::Engine&, int);
	using two_dimensional_function = void(mt::Engine&, int, int);
	using three_dimensional_function = void(mt::Engine&, int, int, int);

	struct InputData1D
	{
		const int x;

		constexpr InputData1D(const int x)
			: x(x)
		{}
	};

	constexpr std::wstring to_wstring(InputData1D input_data_1d)
	{
		return L"{ " + std::to_wstring(input_data_1d.x) + L" }";
	}

	struct InputData2D
	{
		const int x;
		const int y;

		constexpr InputData2D(const int x, const int y)
			: x(x)
			, y(y)
		{}
	};

	constexpr std::wstring to_wstring(InputData2D input_data_2d)
	{
		return L"{ " + std::to_wstring(input_data_2d.x) + L", " + std::to_wstring(input_data_2d.x) + L" }";
	}

	struct InputData3D
	{
		const int x;
		const int y;
		const int z;

		constexpr InputData3D(const int x = 0, const int y = 0, const int z = 0)
			: x(x)
			, y(y)
			, z(z)
		{}
	};

	std::wstring to_wstring(InputData3D input_data_3d)
	{
		return L"{ " + std::to_wstring(input_data_3d.x) + L", " + std::to_wstring(input_data_3d.y) + L", " + std::to_wstring(input_data_3d.z) + L" }";
	}

	// TODO: add 1-3d input as appropriate
	struct InputMessage
	{
		const mt::time::TimePoint	time_point;
		const InputType				input_type;

		const std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data;

		InputMessage(
			const InputType input_type = InputType(),
			const mt::time::TimePoint time_point = mt::time::Clock::now(),
			const std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data = std::monostate()
		)
			: input_type(input_type)
			, time_point(time_point)
			, data(data)
		{

		}
	};

	std::wstring to_wstring(InputMessage input_message)
	{

		switch (input_message.data.index()) {
			case 0:
			{
				return L"{ " + to_wstring(input_message.input_type) + L" }" ;
			}
			case 1:
			{
				auto& data1d = std::get<1>(input_message.data);
				return  L"{ " + to_wstring(input_message.input_type) + L", " + to_wstring(data1d) + L" }";
			}
			case 2:
			{
				auto& data2d = std::get<2>(input_message.data);
				return L"{ " + to_wstring(input_message.input_type) + L", " + to_wstring(data2d) + L" }";
			}
			case 3:
			{
				auto& data3d = std::get<3>(input_message.data);
				return L"{ " + to_wstring(input_message.input_type) + L", " + to_wstring(data3d) + L" }";
			}
		}
		return L"";
	}
}