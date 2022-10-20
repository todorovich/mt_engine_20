// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <DirectXMath.h>
#include <windows.h>

module InputManager;

import DirectXRenderer;
import Engine;
import WindowManager;
import TimeManager;

using namespace mt::input;

void InputManager::processInput()
{
	//v.index(); // which one are we
	auto size = _input_queue.size();
	for (auto x = 0u; x < size; x++)
	{
		std::variant<MouseInputMessage, KeyboardInputMessage>* input_message_variant = _input_queue.front();

		_input_queue.pop();
		
		int index = input_message_variant->index();
		if (index)
		{
			_processKeyboardInput(std::get<KeyboardInputMessage>(*input_message_variant));
		}
		else 
		{
			_processMouseInput(std::get<MouseInputMessage>(*input_message_variant));
		}

		_message_pool.releaseMemory(input_message_variant);
	}
}

// TODO 
void InputManager::_processMouseInput(MouseInputMessage& mouse_input_message)
{
	if (mouse_input_message.left_button)
		_held_buttons.insert(MouseButtons::LEFT);
	else
		_held_buttons.erase(MouseButtons::LEFT);

	if (mouse_input_message.right_button)
		_held_buttons.insert(MouseButtons::RIGHT);
	else
		_held_buttons.erase(MouseButtons::RIGHT);

	if (mouse_input_message.middle_button)
		_held_buttons.insert(MouseButtons::MIDDLE);
	else
		_held_buttons.erase(MouseButtons::MIDDLE);

	if (mouse_input_message.button_4)
		_held_buttons.insert(MouseButtons::FOUR);
	else
		_held_buttons.erase(MouseButtons::FOUR);

	if (mouse_input_message.button_5)
		_held_buttons.insert(MouseButtons::FIVE);
	else
		_held_buttons.erase(MouseButtons::FIVE);

	 //Left mouse button is being held
	if (_held_buttons.find(MouseButtons::LEFT) != _held_buttons.end())
	{
		auto& camera = _engine.getRenderer()->getCurrentCamera();
	
		// Make each pixel correspond to 1/10th of a degree.
		float dx = DirectX::XMConvertToRadians(0.1f*static_cast<float>(mouse_input_message.x - _mouse_position.x));
		float dy = DirectX::XMConvertToRadians(0.1f*static_cast<float>(mouse_input_message.y - _mouse_position.y));
	
		camera.pitch(dy);
		camera.rotateY(dx);
	}
	
	_mouse_position.x = mouse_input_message.x;
	_mouse_position.y = mouse_input_message.y;
}

void InputManager::_processKeyboardInput(KeyboardInputMessage& keyboard_input_message)
{
	switch (keyboard_input_message.key_state)
	{
		case KeyState::PRESSED: [[fallthrough]];
		[[likely]] case KeyState::HELD: _held_keys.insert(keyboard_input_message.key); break;
		case KeyState::RELEASED: _held_keys.erase(keyboard_input_message.key); break;
		[[unlikely]] case KeyState::NO_STATE: _held_keys.clear(); break;
	}

	OutputDebugStringW((std::wstring(to_wstring(keyboard_input_message.key)) + L" " + std::wstring(to_wstring(keyboard_input_message.key_state)) + L'\n').c_str());
	
	if (keyboard_input_message.key == KeyboardKeys::ESCAPE)
	{
		PostMessage(_engine.getWindowManager()->getMainWindowHandle(), WM_CLOSE, 0, 0);
	}
	else if (keyboard_input_message.key == KeyboardKeys::PAUSE)
	{
		if (mt::time::TimeManager& time_manager = *_engine.getTimeManager(); time_manager.IsUpdatePaused())
		{
			time_manager.resume();
		}
		else
		{
			time_manager.pause();
		}
	}
}

void InputManager::keyboardEvent(KeyboardKeys key, KeyState key_state)
{
	auto ptr = _message_pool.getMemory();
	ptr->emplace<KeyboardInputMessage>(this, key, key_state);
	_input_queue.push(ptr);
}

void InputManager::mouseEvent(__int32 x, __int32 y, bool left_button, bool right_button, bool middle_button, bool button_4, bool button_5)
{
	auto ptr = _message_pool.getMemory();
	ptr->emplace<MouseInputMessage>(this, x, y, left_button, right_button, middle_button, button_4, button_5);
	_input_queue.push(ptr);
}
