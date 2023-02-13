// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <DirectXMath.h>
#include <windows.h>

#undef RELATIVE

module BasicInputManager;

import Engine;
import InputModel;

using namespace gsl;
using namespace mt::input;
using namespace mt::input::model;
using namespace mt::task;

using namespace std::literals;

void BasicInputManager::processInput() noexcept
{
	auto size = _input_queue.size();

	std::set<InputType> pressed_buttons{};

	auto default_constructed_input_type = InputType();

	for (auto x = 0u; x < size; x++)
	{
		InputMessage& input_message = *_input_queue.front();

		const auto& input_type = input_message.input_type;

		// Lost focus should clear held keys.
		if (input_type == default_constructed_input_type) {

			for (auto held_button : _held_buttons)
			{
				const auto released_input_type =
					InputType(held_button.input_device, InputDataType::BUTTON_RELEASED, held_button.input_context, held_button.virtual_key_code);

				const auto released_range = button_input_handler.equal_range(released_input_type);

				for (auto released_it = released_range.first; released_it != released_range.second; ++released_it)
				{
					(*released_it->second)();
				}
			}
			_held_buttons.clear();
			_input_queue.pop();
			continue;
		}

		const auto& input_data_type = input_type.input_data_type;

		switch (input_data_type)
		{
			// check if button is in held buttons
			// if so then trigger button released action and remove it
			// if not trigger button idle action
			case InputDataType::BUTTON_IDLE: [[fallthrough]];
			case InputDataType::BUTTON_RELEASED:
			{
				const auto held_input_type = InputType(
					input_type.input_device,	InputDataType::BUTTON_HELD,
					input_type.input_context,	input_type.virtual_key_code
				);

				if (auto held_it = _held_buttons.find(held_input_type); held_it != _held_buttons.end())
				{
					const auto released_input_type =
						InputType(input_type.input_device, InputDataType::BUTTON_RELEASED, input_type.input_context, input_type.virtual_key_code);

					const auto released_range = button_input_handler.equal_range(released_input_type);

					for (auto released_it = released_range.first; released_it != released_range.second; ++released_it)
					{
						(*released_it->second)();
					}

					_held_buttons.erase(held_it);
				}
				else
				{
					const auto idle_input_type =
						InputType(input_type.input_device, InputDataType::BUTTON_IDLE, input_type.input_context, input_type.virtual_key_code);

					const auto idle_range = button_input_handler.equal_range(idle_input_type);

					for (auto idle_it = idle_range.first; idle_it != idle_range.second; ++idle_it)
					{
						(*idle_it->second)();
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
				const auto held_input_type =
					InputType(input_type.input_device, InputDataType::BUTTON_HELD, input_type.input_context, input_type.virtual_key_code);

				// NOT HELD
				if (auto held_it = _held_buttons.find(held_input_type); held_it == _held_buttons.end())
				{
					const auto pressed_input_type =
						InputType(input_type.input_device, InputDataType::BUTTON_PRESSED, input_type.input_context, input_type.virtual_key_code);

					const auto pressed_range = button_input_handler.equal_range(pressed_input_type);

					for (auto pressed_it = pressed_range.first; pressed_it != pressed_range.second; ++pressed_it)
					{
						(*pressed_it->second)();
					}

					pressed_buttons.insert(held_input_type);
				}
				// ELSE (HELD) no op, held buttons will be executed after all input has been processed. To allow for release to happen, and to override held.
			}
				break;

			case InputDataType::ONE_DIMENSIONAL:
			{
				const auto range = one_dimensional_input_handler.equal_range(input_type);

				for (auto it = range.first; it != range.second; ++it)
				{
					InputData1D input_data = std::get<InputData1D>(input_message.data);
					(*it->second)(input_data.x);
				}
			}
				break;

			case InputDataType::TWO_DIMENSIONAL:
			{
				const auto& data2d = std::get<InputData2D>(input_message.data);

				if (getIsMouseRelative() && input_type.input_device == InputDevice::MOUSE)
				{
					const auto relative_mouse_input_type = InputType(
						InputDevice::MOUSE, InputDataType::TWO_DIMENSIONAL, InputContext::RELATIVE
					);

					const auto range = two_dimensional_input_handler.equal_range(relative_mouse_input_type);

					const int half_width = _engine.getWindowManager()->getWindowWidth() / 2;
					const int half_height = _engine.getWindowManager()->getWindowHeight() / 2;

					for (auto it = range.first; it != range.second; ++it)
					{
						(*it->second)(data2d.x - half_width, data2d.y - half_height);
					}

					SetCursorPos(half_width, half_height);
				}
				else
				{
					const auto range = two_dimensional_input_handler.equal_range(input_type);

					for (auto it = range.first; it != range.second; ++it)
					{
						(*it->second)(data2d.x, data2d.y);
					}
				}
			}
				break;

			case InputDataType::THREE_DIMENSIONAL:
			{
				const auto range = three_dimensional_input_handler.equal_range(input_type);
				const auto& data3d = std::get<InputData3D>(input_message.data);

				for (auto it = range.first; it != range.second; ++it)
				{
					(*it->second)(data3d.x, data3d.y, data3d.z);
				}
			}
				break;

			case InputDataType::
		}

		// This causes the smart pointer to release
		_input_queue.pop();
	}

	// Trigger the held buttons.
	for (auto input_type : _held_buttons)
	{
		if (auto it = button_input_handler.find(input_type); it != button_input_handler.end())
			(*it->second)();
	}

	_held_buttons.merge(pressed_buttons);

	if (_message_pool.size() < _message_pool.capacity())
		is_accepting_input.store(true);
}

void mt::input::BasicInputManager::acceptInput(
	InputType input_type, std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data
) noexcept
{
	if (isAcceptingInput()){
		if (auto pointer = _message_pool.allocate(input_type, std::chrono::steady_clock::now(), data); pointer)
		{
			_input_queue.push(std::move(pointer));

			if (_message_pool.size() == _message_pool.capacity())
				is_accepting_input.store(false);
		}
		else {
			_engine.crash(mt::error::Error{
				L"Failed to accept input, message queue is full."sv,
				mt::error::ErrorCode::UNABLE_TO_ACCEPT_INPUT,
				__func__, __FILE__, __LINE__
			});
		}
	}
	else {
		_engine.crash(mt::error::Error{
			L"Input Manager is full and not accepting input."sv,
			mt::error::ErrorCode::CALLED_WHILE_NOT_ACCEPTING_INPUT,
			__func__, __FILE__, __LINE__
		});
	}
}

void mt::input::BasicInputManager::toggleRelativeMouse() noexcept
{
	if (getIsMouseRelative())
	{
		setIsMouseRelative(false);

		ShowCursor(true);

		SetCursorPos(_mouse_return_position.x, _mouse_return_position.y);
	}
	else
	{
		setIsMouseRelative();

		const int half_width = _engine.getWindowManager()->getWindowWidth() / 2;
		const int half_height = _engine.getWindowManager()->getWindowHeight() / 2;

		GetCursorPos(&_mouse_return_position);

		SetCursorPos(half_width, half_height);

		ShowCursor(false);
	}
}

void mt::input::BasicInputManager::registerInputHandler(InputHandler input_handler, InputType input_type) noexcept
{
	switch (input_type.input_data_type)
	{
		case InputDataType::BUTTON_IDLE: [[fallthrough]];
		case InputDataType::BUTTON_PRESSED: [[fallthrough]];
		case InputDataType::BUTTON_HELD: [[fallthrough]];
		case InputDataType::BUTTON_RELEASED:
		{
			auto& button_handling_function = std::get<not_null<Task*>>(input_handler);
			button_input_handler.insert({input_type, button_handling_function});
		}
			break;

		case InputDataType::ONE_DIMENSIONAL:
		{
			auto& one_dimensional_handling_function =
			std::get<not_null<OneDimensionalInputTask*>>(input_handler);
			one_dimensional_input_handler.insert({input_type, one_dimensional_handling_function});
		}
			break;

		case InputDataType::TWO_DIMENSIONAL:
		{
			auto& two_dimensional_handling_function =
			std::get<not_null<TwoDimensionalInputTask*>>(input_handler);
			two_dimensional_input_handler.insert({input_type, two_dimensional_handling_function});
		}
			break;

		case InputDataType::THREE_DIMENSIONAL:
		{
			auto& three_dimensional_handling_function =
			std::get<not_null<ThreeDimensionalInputTask*>>(input_handler);
			three_dimensional_input_handler.insert({input_type, three_dimensional_handling_function});
		}
			break;

		case InputDataType::NO_DATA_TYPE:
			_engine.crash(mt::error::Error{
				L"Attempted to register InputType with InputDataType::NO_DATA_TYPE which is invalid."sv,
				mt::error::ErrorCode::INVALID_INPUT_DATA_TYPE,
				__func__, __FILE__, __LINE__
			});
			break;
	}
}