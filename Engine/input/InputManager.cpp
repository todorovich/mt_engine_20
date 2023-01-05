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

void InputManager::processInput() noexcept
{
	auto size = _input_queue.size();

	std::set<InputType> pressed_buttons{};

	for (auto x = 0u; x < size; x++)
	{
		InputMessage* input_message = _input_queue.front();

		auto& input_type = input_message->input_type;

		auto& input_data_type = input_type.input_data_type;

		switch (input_data_type)
		{
			// check if button is in held buttons
			// if so then trigger button released action and remove it
			// if not trigger button idle action
			case InputDataType::BUTTON_IDLE: [[fallthrough]]; 
			case InputDataType::BUTTON_RELEASED: 
			{
				auto held_input_type =
					InputType(input_type.input_device, InputDataType::BUTTON_HELD, input_type.input_context, input_type.virtual_key_code);

				if (auto held_it = _held_buttons.find(held_input_type); held_it != _held_buttons.end())
				{
					auto released_input_type =
						InputType(input_type.input_device, InputDataType::BUTTON_RELEASED, input_type.input_context, input_type.virtual_key_code);

					auto released_range = button_input_handler.equal_range(released_input_type);

					for (auto released_it = released_range.first; released_it != released_range.second; ++released_it)
					{
						released_it->second(_engine);
					}

					_held_buttons.erase(held_it);
				}
				else
				{
					auto idle_input_type =
						InputType(input_type.input_device, InputDataType::BUTTON_IDLE, input_type.input_context, input_type.virtual_key_code);

					auto idle_range = button_input_handler.equal_range(idle_input_type);

					for (auto idle_it = idle_range.first; idle_it != idle_range.second; ++idle_it)
					{
						idle_it->second(_engine);
					}
				}
			}
			break;

			// check if button is in held buttons
			// if so then noop (Wait for held buttons loop to trigger action)
			// if not then trigger button pressed and put button in set to be added to held buttons after the action loop
			case InputDataType::BUTTON_HELD: [[fallthrough]]; 
			case InputDataType::BUTTON_PRESSED: 
			{
				auto held_input_type =
					InputType(input_type.input_device, InputDataType::BUTTON_HELD, input_type.input_context, input_type.virtual_key_code);

				// NOT HELD
				if (auto held_it = _held_buttons.find(held_input_type); held_it == _held_buttons.end())
				{
					auto pressed_input_type =
						InputType(input_type.input_device, InputDataType::BUTTON_PRESSED, input_type.input_context, input_type.virtual_key_code);

					auto pressed_range = button_input_handler.equal_range(pressed_input_type);

					for (auto pressed_it = pressed_range.first; pressed_it != pressed_range.second; ++pressed_it)
					{
						pressed_it->second(_engine);
					}

					pressed_buttons.insert(held_input_type);
				}
				// ELSE (HELD) no op, held buttons will be executed after all input has been processed. To allow for release to happen, and to override held.
			}
			break;
			
			case InputDataType::ONE_DIMENSIONAL:
			{
				auto range = one_dimensional_input_handler.equal_range(input_type);

				for (auto it = range.first; it != range.second; ++it)
				{
					InputData1D input_data = std::get<mt::input::InputData1D>(input_message->data);
					it->second(_engine, input_data.x);
				}
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

					auto range = two_dimensional_input_handler.equal_range(relative_mouse_input_type);

					const int half_width = _engine.getRenderer()->getWindowWidth() / 2;
					const int half_height = _engine.getRenderer()->getWindowHeight() / 2;

					for (auto it = range.first; it != range.second; ++it)
					{
						it->second(_engine, data2d.x - half_width, data2d.y - half_height);
					}

					SetCursorPos(half_width, half_height);
				}
				else
				{
					auto range = two_dimensional_input_handler.equal_range(input_type);

					for (auto it = range.first; it != range.second; ++it)
					{
						it->second(_engine, data2d.x, data2d.y);
					}
				}
			}
			break;

			case InputDataType::THREE_DIMENSIONAL:
			{
				auto range = three_dimensional_input_handler.equal_range(input_type);
				auto& data3d = std::get<InputData3D>(input_message->data);

				for (auto it = range.first; it != range.second; ++it)
				{
					it->second(_engine, data3d.x, data3d.y, data3d.z);
				}
			}
			break;
		}

		_input_queue.pop();

		_message_pool.releaseMemory(input_message);
	}

	// Trigger the held buttons.
	for (auto input_type : _held_buttons)
	{
		if (auto it = button_input_handler.find(input_type); it != button_input_handler.end())
			it->second(_engine);
	}
	
	_held_buttons.merge(pressed_buttons);
}

void mt::input::InputManager::acceptInput(
	InputType input_type, std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data
) noexcept
{
	_input_queue.push(_message_pool.allocate(input_type, std::chrono::steady_clock::now(), data));
}

void mt::input::InputManager::toggleRelativeMouse() noexcept
{
	if (isMouseRelative)
	{
		isMouseRelative = false;

		ShowCursor(true);
	
		SetCursorPos(_mouse_return_position.x, _mouse_return_position.y);
	}
	else
	{
		isMouseRelative = true;
		
		const int half_width = _engine.getRenderer()->getWindowWidth() / 2;
		const int half_height = _engine.getRenderer()->getWindowHeight() / 2;

		GetCursorPos(&_mouse_return_position);

		SetCursorPos(half_width, half_height);

		ShowCursor(false);
	}
}

void mt::input::InputManager::registerInputHandler(InputHandler input_handler, InputType input_type) noexcept
{
	switch (input_type.input_data_type)
	{
		case InputDataType::BUTTON_IDLE: [[fallthrough]];
		case InputDataType::BUTTON_PRESSED: [[fallthrough]];
		case InputDataType::BUTTON_HELD: [[fallthrough]];
		case InputDataType::BUTTON_RELEASED:
		{
			auto button_handling_function = std::get<button_function*>(input_handler);
			button_input_handler.insert({input_type, button_handling_function});
		}
		break;

		case InputDataType::ONE_DIMENSIONAL:
		{
			auto one_dimensional_handling_function = std::get<one_dimensional_function*>(input_handler);
			one_dimensional_input_handler.insert({input_type, one_dimensional_handling_function});
		}
		break;

		case InputDataType::TWO_DIMENSIONAL:
		{
			auto two_dimensional_handling_function = std::get<two_dimensional_function*>(input_handler);
			two_dimensional_input_handler.insert({input_type, two_dimensional_handling_function});
		}
		break;

		case InputDataType::THREE_DIMENSIONAL:
		{
			auto three_dimensional_handling_function = std::get<three_dimensional_function*>(input_handler);
			three_dimensional_input_handler.insert({input_type, three_dimensional_handling_function});
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
