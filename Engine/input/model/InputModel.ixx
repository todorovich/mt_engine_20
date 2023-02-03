// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <Windows.h>

export module InputModel;

export import <string>;
export import <variant>;
export import <chrono>;

export import gsl;
export import InputContext;
export import InputDataType;
export import InputDevice;
export import InputType;
export import MicrosoftVirtualKeyCode;
export import VirtualKeyCode;
export import Task;

export using namespace std::literals;

using namespace gsl;
using namespace mt::input;
using namespace mt::task;

export namespace mt::input::model
{


	using InputHandler = std::variant<
		not_null<Task*>,
		not_null<OneDimensionalInputTask*>,
		not_null<TwoDimensionalInputTask*>,
		not_null<ThreeDimensionalInputTask*>
	>;

	struct InputData1D
	{
		const int x;

		constexpr InputData1D(const int x) noexcept
			: x(x)
		{}
	};

	constexpr std::wstring to_wstring(InputData1D input_data_1d) noexcept
	{
		return L"{ " + std::to_wstring(input_data_1d.x) + L" }";
	}

	struct InputData2D
	{
		const int x;
		const int y;

		constexpr InputData2D(const int x, const int y) noexcept
			: x(x)
			, y(y)
		{}
	};

	constexpr std::wstring to_wstring(InputData2D input_data_2d) noexcept
	{
		return L"{ " + std::to_wstring(input_data_2d.x) + L", " + std::to_wstring(input_data_2d.x) + L" }";
	}

	struct InputData3D
	{
		const int x;
		const int y;
		const int z;

		constexpr InputData3D(const int x = 0, const int y = 0, const int z = 0) noexcept
			: x(x)
			, y(y)
			, z(z)
		{}
	};

	std::wstring to_wstring(InputData3D input_data_3d) noexcept
	{
		return L"{ " + std::to_wstring(input_data_3d.x) + L", " + std::to_wstring(input_data_3d.y) + L", " + std::to_wstring(input_data_3d.z) + L" }";
	}

	struct InputMessage
	{
		const std::chrono::steady_clock::time_point	time_point;
		const InputType				input_type;

		const std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data;

		InputMessage(
			const InputType input_type = InputType(),
			const std::chrono::steady_clock::time_point time_point = std::chrono::steady_clock::now(),
			const std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data = std::monostate()
		) noexcept
			: input_type(input_type)
			, time_point(time_point)
			, data(data)
		{

		}
	};

	std::wstring to_wstring(InputMessage input_message) noexcept
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