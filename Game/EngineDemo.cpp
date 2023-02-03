// Copyright 2023 Micho Todorovich, all rights reserved.
module;
#include <DirectXMath.h>
#include <Windows.h>
// Fuck macros. This wasted like 30 minutes of my time.
#undef RELATIVE

module EngineDemo;

import <chrono>;
import <functional>; // std::hash

import gsl;

using namespace mt;
using namespace mt::input;

using namespace std::literals;

using mt::input::InputDevice::KEYBOARD;
using mt::input::InputDevice::MOUSE;

using mt::input::InputDataType::BUTTON_PRESSED;
using mt::input::InputDataType::BUTTON_RELEASED;
using mt::input::InputDataType::BUTTON_HELD;
using mt::input::InputDataType::TWO_DIMENSIONAL;

using mt::input::InputContext::NO_CONTEXT;
using mt::input::InputContext::RELATIVE;

using mt::input::VirtualKeyCode::ESCAPE;

const std::size_t mt::Keys::QUIT = std::hash<std::wstring>()(L"QUIT");
const std::size_t mt::Keys::TOGGLE_RELATIVE_MOUSE = std::hash<std::wstring>()(L"TOGGLE_RELATIVE_MOUSE");
const std::size_t mt::Keys::WALK_FORWARD = std::hash<std::wstring>()(L"WALK_FORWARD");
const std::size_t mt::Keys::WALK_BACKWARD = std::hash<std::wstring>()(L"WALK_BACKWARD");
const std::size_t mt::Keys::STRAFE_LEFT = std::hash<std::wstring>()(L"STRAFE_LEFT");
const std::size_t mt::Keys::STRAFE_RIGHT = std::hash<std::wstring>()(L"STRAFE_RIGHT");
const std::size_t mt::Keys::FLY_UP = std::hash<std::wstring>()(L"FLY_UP");
const std::size_t mt::Keys::FLY_DOWN = std::hash<std::wstring>()(L"FLY_DOWN");

void mt::EngineDemo::map_input_controls() noexcept
{
	_engine.getInputManager()->registerInputHandler(
		_tasks.find(Keys::QUIT)->second.get(),
		{ KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, ESCAPE }
	);

	_engine.getInputManager()->registerInputHandler(
		_tasks.find(Keys::TOGGLE_RELATIVE_MOUSE)->second.get(),
		InputType(MOUSE, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::ONE),
		InputType(MOUSE, BUTTON_RELEASED, NO_CONTEXT, VirtualKeyCode::ONE)
	);

	_engine.getInputManager()->registerInputHandler(
		_tasks.find(Keys::WALK_FORWARD)->second.get(),
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::W),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::W)
	);

	_engine.getInputManager()->registerInputHandler(
		_tasks.find(Keys::WALK_BACKWARD)->second.get(),
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::S),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::S)
	);

	_engine.getInputManager()->registerInputHandler(
		_tasks.find(Keys::STRAFE_LEFT)->second.get(),
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::A),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::A)
	);

	_engine.getInputManager()->registerInputHandler(
		_tasks.find(Keys::STRAFE_RIGHT)->second.get(),
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::D),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::D)
	);

	_engine.getInputManager()->registerInputHandler(
		_tasks.find(Keys::FLY_UP)->second.get(),
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::SPACE),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::SPACE)
	);

	_engine.getInputManager()->registerInputHandler(
		_tasks.find(Keys::FLY_DOWN)->second.get(),
		InputType(KEYBOARD, BUTTON_PRESSED, NO_CONTEXT, VirtualKeyCode::CONTROL),
		InputType(KEYBOARD, BUTTON_HELD, 	NO_CONTEXT, VirtualKeyCode::CONTROL)
	);

	_engine.getInputManager()->registerInputHandler(
		_mouse_look.get(),
		InputType(MOUSE, TWO_DIMENSIONAL, RELATIVE, VirtualKeyCode::NO_KEY)
	);
}