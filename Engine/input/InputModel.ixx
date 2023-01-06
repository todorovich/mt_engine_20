// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module InputModel;

export import <string>;
export import <variant>;
export import <chrono>;

export import InputContext;
export import InputDataType;
export import InputDevice;
export import InputType;
export import MicrosoftVirtualKeyCode;
export import VirtualKeyCode;

export using namespace std::literals;

export namespace mt { class Engine; }

using namespace mt::input;

export namespace mt::input
{
	using button_function = void(mt::Engine&) noexcept;
	using one_dimensional_function = void(mt::Engine&, int) noexcept;
	using two_dimensional_function = void(mt::Engine&, int, int) noexcept;
	using three_dimensional_function = void(mt::Engine&, int, int, int) noexcept;

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

	// TODO: add 1-3d input as appropriate
	struct InputMessage
	{
		const std::chrono::steady_clock::time_point	time_point;
		const mt::input::InputType				input_type;

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