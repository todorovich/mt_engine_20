// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <DirectXMath.h>
#include <windows.h>

#undef RELATIVE

module InputManager;

import DirectXRenderer;
import Engine;
import WindowManager;
import TimeManager;
import InputModel;

using namespace mt::input;

void InputManager::processInput()
{
	auto size = _input_queue.size();
	for (auto x = 0u; x < size; x++)
	{
		InputMessage* input_message = _input_queue.front();

		auto& input_type = input_message->input_type;

		auto& input_data_type = input_type.input_data_type;

		switch (input_data_type)
		{
			// How do we register actions? Reregister given context individually into IDLE/PRESSED/HELD/RELEASED?
			// this would allow to search for only the one that happened, and use that, but could overlap another context?
			// eg register press and pressed|released for A. What happens on conflict? Multimap? In order to tell what to unregister
			// wed have to not modify the inputtype at all if this double reg is valid. 
			case InputDataType::BUTTON_IDLE: [[fallthrough]];
			case InputDataType::BUTTON_PRESSED: [[fallthrough]];
			case InputDataType::BUTTON_HELD: [[fallthrough]];
			case InputDataType::BUTTON_RELEASED:
			{
				if (auto it = button_input_handler.find(input_type); it != button_input_handler.end()) it->second();
				
				if constexpr (mt::DEBUG)
				{
					auto s = to_wstring(*input_message) + L'\n';
					if constexpr (mt::DEBUG) OutputDebugStringW(s.c_str());
				}
			}
				break;
			
			case InputDataType::ONE_DIMENSIONAL:
				if (auto it = one_dimensional_input_handler.find(input_type); it != one_dimensional_input_handler.end())
				{
					InputData1D input_data = std::get<mt::input::InputData1D>(input_message->data);
					it->second(input_data.x);
				}

				if constexpr (mt::DEBUG)
				{
					auto s = to_wstring(*input_message) + L'\n';
					if constexpr (mt::DEBUG) OutputDebugStringW(s.c_str());
				}
				break;

			case InputDataType::TWO_DIMENSIONAL:
			{
				auto& data2d = std::get<mt::input::InputData2D>(input_message->data);

				if (isMouseRelative && input_type.input_device == mt::input::InputDevice::MOUSE)
				{

					auto relative_mouse_input_type = mt::input::InputType(
						mt::input::InputDevice::MOUSE, mt::input::InputDataType::TWO_DIMENSIONAL, mt::input::InputContext::RELATIVE
					);

					if (auto it = two_dimensional_input_handler.find(relative_mouse_input_type); it != two_dimensional_input_handler.end())
					{
						const int half_width = _engine.getRenderer()->getWindowWidth() / 2;
						const int half_height = _engine.getRenderer()->getWindowHeight() / 2;

						SetCursorPos(half_width, half_height);

						it->second(data2d.x - half_width, data2d.y - half_height);
					}
				}
				else
				{
					if (auto it = two_dimensional_input_handler.find(input_type); it != two_dimensional_input_handler.end())
					{
						it->second(data2d.x, data2d.y);
					}
				}
			}
				break;

			case InputDataType::THREE_DIMENSIONAL:
				if (auto it = three_dimensional_input_handler.find(input_type); it != three_dimensional_input_handler.end())
				{
					auto data3d = std::get<InputData3D>(input_message->data);
					it->second(data3d.x, data3d.y, data3d.z);
				}
				break;
		}

		_input_queue.pop();

		_message_pool.releaseMemory(input_message);
	}
}

void mt::input::InputManager::acceptInput(InputType input_type, std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data)
{
	_input_queue.push(
		new (_message_pool.getMemory()) InputMessage(input_type, mt::time::Clock::now(), data)
	);
}


void mt::input::InputManager::registerInputHandler(InputType input_type, InputHandler input_handler)
{
	switch (input_type.input_data_type)
	{
		case InputDataType::BUTTON_IDLE: [[fallthrough]];
		case InputDataType::BUTTON_PRESSED: [[fallthrough]];
		case InputDataType::BUTTON_HELD: [[fallthrough]];
		case InputDataType::BUTTON_RELEASED:
		{
			auto& button_handling_function = std::get<std::function<void()>>(input_handler);
			button_input_handler.insert({input_type, button_handling_function});
		}
		break;

		case InputDataType::ONE_DIMENSIONAL:
		{
			auto& one_dimenaional_handling_function = std::get<std::function<void(int)>>(input_handler);
			one_dimensional_input_handler.insert({input_type, one_dimenaional_handling_function});
		}
		break;

		case InputDataType::TWO_DIMENSIONAL:
		{
			auto& two_dimenaional_handling_function = std::get<std::function<void(int, int)>>(input_handler);
			two_dimensional_input_handler.insert({input_type, two_dimenaional_handling_function});
		}
		break;

		case InputDataType::THREE_DIMENSIONAL:
		{
			auto& three_dimenaional_handling_function = std::get<std::function<void(int, int, int)>>(input_handler);
			three_dimensional_input_handler.insert({input_type, three_dimenaional_handling_function});
		}
		break;

		case InputDataType::NO_DATA_TYPE: break; // THROW HERE?
	}
}

//	else if (keyboard_input_message.button_input.button == InputButton::PAUSE)
//	{
//		if (mt::time::TimeManager& time_manager = *_engine.getTimeManager(); time_manager.IsUpdatePaused())
//		{
//			time_manager.resume();
//		}
//		else
//		{
//			time_manager.pause();
//		}
//	}
//}
