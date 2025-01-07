// Copyright 2023 Micho Todorovich, all rights reserved.
module BasicInputManager;

import std;
import Engine;
import InputManagerInterface;
import InputModel;
import Windows;

using namespace windows;
using namespace gsl;
using namespace mt::error;
using namespace mt::input;
using namespace mt::input::model;
using namespace mt::task;

using namespace std::literals;

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void BasicInputManager::processInput() noexcept
{
	auto size = _input_queue.size();

	std::set<InputType> pressed_buttons{};

	auto default_constructed_input_type = InputType();

	for (auto x = 0u; x < size; x++)
	{
		InputMessage& input_message = *_input_queue.front();

		const auto& input_type = input_message.input_type;

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
			case InputDataType::TWO_DIMENSIONAL:
			case InputDataType::THREE_DIMENSIONAL:
				std::visit(
					overloaded{
						[](std::monostate) {},
						[&](const InputData1D& input_data)
						{
							const auto range = one_dimensional_input_handler.equal_range(input_type);
							for (auto it = range.first; it != range.second; ++it)
							{
								(*it->second)(input_data.x);
							}
						},
						[&](const InputData2D& input_data)
						{
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
									(*it->second)(input_data.x - half_width, input_data.y - half_height);
								}
								SetCursorPos(half_width, half_height);
							}
							else
							{
								const auto range = two_dimensional_input_handler.equal_range(input_type);
								for (auto it = range.first; it != range.second; ++it)
								{
									(*it->second)(input_data.x, input_data.y);
								}
							}
						},
						[&](const InputData3D& input_data)
						{
							const auto range = three_dimensional_input_handler.equal_range(input_type);
							for (auto it = range.first; it != range.second; ++it)
							{
								(*it->second)(input_data.x, input_data.y, input_data.z);
							}
						}
					},
					input_message.data
				);
				break;
			

			case InputDataType::NO_DATA_TYPE:
			{
				// Lost focus should clear held keys.
				if (input_type == InputType())
				{

					for (auto held_button: _held_buttons)
					{
						const auto released_input_type =
							InputType(
								held_button.input_device, InputDataType::BUTTON_RELEASED, held_button.input_context,
								held_button.virtual_key_code
							);

						const auto released_range = button_input_handler.equal_range(released_input_type);

						for (auto released_it = released_range.first; released_it != released_range.second;
							++released_it)
						{
							(*released_it->second)();
						}
					}
					_held_buttons.clear();
					_input_queue.pop();
					continue;
				}
				else if (input_type == InputType(
					InputDevice::MOUSE, InputDataType::NO_DATA_TYPE, InputContext::RELATIVE
				))
				{
					toggleIsMouseRelative();
				}
			}
				break;
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

std::expected<void, std::error_condition> mt::input::BasicInputManager::acceptInput(
	InputType input_type, std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data
) noexcept
{
	if (isAcceptingInput()){
		if (auto expected_pointer = _message_pool.allocate(input_type, std::chrono::steady_clock::now(), data); expected_pointer)
		{
			auto&& pointer = expected_pointer.value();

			_input_queue.push(std::move(pointer));

			if (_message_pool.size() == _message_pool.capacity())
				is_accepting_input.store(false);
		}
		else {
			return std::unexpected{ _engine.crash(MakeErrorCondition(ErrorCode::UNABLE_TO_ACCEPT_INPUT)) };
		}
	}
	else {
		return std::unexpected{ _engine.crash(MakeErrorCondition(mt::error::ErrorCode::CALLED_WHILE_NOT_ACCEPTING_INPUT)) };
	}

	return {};
}

void mt::input::BasicInputManager::toggleIsMouseRelative() noexcept
{
	if (InputManagerInterface::getIsMouseRelative())
	{
		InputManagerInterface::_setIsMouseRelative(false);

		_engine.getWindowManager()->toggleShowCursor();

		SetCursorPos(_mouse_return_position.x, _mouse_return_position.y);
	}
	else
	{
		InputManagerInterface::_setIsMouseRelative();

		const int half_width = _engine.getWindowManager()->getWindowWidth() / 2;
		const int half_height = _engine.getWindowManager()->getWindowHeight() / 2;

		GetCursorPos(&_mouse_return_position);

		SetCursorPos(half_width, half_height);

		_engine.getWindowManager()->toggleShowCursor();
	}
}

void mt::input::BasicInputManager::registerInputHandler(InputHandler input_handler, InputType input_type) noexcept
{
    std::visit(
        overloaded {
            [&](Task * const  task) -> void { 
				const auto comparator = [&](const auto& pair) { return pair.first == input_type && pair.second == task; };
                if (
					auto result = std::find_if(button_input_handler.begin(), button_input_handler.end(), comparator);
					result == button_input_handler.end()
				)
                {
                    button_input_handler.insert({ input_type, task });
                }
            },
            [&](OneDimensionalInputTask * const  task) -> void {
				const auto comparator = [&](const auto& pair) { return pair.first == input_type && pair.second == task; };
                if (
					auto result = std::find_if(one_dimensional_input_handler.begin(), one_dimensional_input_handler.end(), comparator);
					result == one_dimensional_input_handler.end()
				)
                {
                    one_dimensional_input_handler.insert({ input_type, task });
                }
            },
            [&](TwoDimensionalInputTask * const  task) -> void { 
				const auto comparator = [&](const auto& pair) { return pair.first == input_type && pair.second == task; };
                if (
					auto result = std::find_if(two_dimensional_input_handler.begin(), two_dimensional_input_handler.end(), comparator);
					result == two_dimensional_input_handler.end()
				)
                {
                    two_dimensional_input_handler.insert({ input_type, task });
                }
            },
            [&](ThreeDimensionalInputTask * const  task) -> void {
				const auto comparator = [&](const auto& pair) { return pair.first == input_type && pair.second == task; };
                if (
					auto result = std::find_if(three_dimensional_input_handler.begin(), three_dimensional_input_handler.end(), comparator);
					result == three_dimensional_input_handler.end()
				)
                {
                    three_dimensional_input_handler.insert({ input_type, task });
                }
            },
        },
        input_handler
    );
}

void mt::input::BasicInputManager::unregisterInputHandler(InputHandler input_handler, InputType input_type) noexcept
{
	std::visit(
		overloaded{
			[&](Task* const task) -> void {
				auto range = button_input_handler.equal_range(input_type);
				for (auto it = range.first; it != range.second; )
				{
					if (it->second == task)
					{
						it = button_input_handler.erase(it);
					}
					else
					{
						++it;
					}
				}
			},
			[&](OneDimensionalInputTask* const task) -> void {
				auto range = one_dimensional_input_handler.equal_range(input_type);
				for (auto it = range.first; it != range.second; )
				{
					if (it->second == task)
					{
						it = one_dimensional_input_handler.erase(it);
					}
					else
					{
						++it;
					}
				}
			},
			[&](TwoDimensionalInputTask* const task) -> void {
				auto range = two_dimensional_input_handler.equal_range(input_type);
				for (auto it = range.first; it != range.second; )
				{
					if (it->second == task)
					{
						it = two_dimensional_input_handler.erase(it);
					}
					else
					{
						++it;
					}
				}
			},
			[&](ThreeDimensionalInputTask* const task) -> void {
				auto range = three_dimensional_input_handler.equal_range(input_type);
				for (auto it = range.first; it != range.second; )
				{
					if (it->second == task)
					{
						it = three_dimensional_input_handler.erase(it);
					}
					else
					{
						++it;
					}
				}
			},
		},
		input_handler
	);
}