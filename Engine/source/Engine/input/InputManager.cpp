// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <DirectXMath.h>
#include <windows.h>

module InputManager;

import Engine;
import DirectXRenderer;
import TimeManager;

using namespace mt::input;

void InputManager::ProcessInput()
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
			_ProcessKeyboardInput(std::get<KeyboardInputMessage>(*input_message_variant));
		}
		else 
		{
			_ProcessMouseInput(std::get<MouseInputMessage>(*input_message_variant));
		}

		_message_pool.ReleaseMemory(input_message_variant);
	}
}

void InputManager::_ProcessMouseInput(MouseInputMessage& mouse_input_message)
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
		auto& camera = _engine.GetRenderer()->GetCurrentCamera();
	
		// Make each pixel correspond to 1/10th of a degree.
		float dx = DirectX::XMConvertToRadians(0.1f*static_cast<float>(mouse_input_message.x - _mouse_position.x));
		float dy = DirectX::XMConvertToRadians(0.1f*static_cast<float>(mouse_input_message.y - _mouse_position.y));
	
		camera.Pitch(dy);
		camera.RotateY(dx);
	}
	
	_mouse_position.x = mouse_input_message.x;
	_mouse_position.y = mouse_input_message.y;
}

void InputManager::_ProcessKeyboardInput(KeyboardInputMessage& keyboard_input_message)
{
	if (keyboard_input_message.key_state == KeyState::PRESSED)
		_held_keys.insert(keyboard_input_message.key);
	else
		_held_keys.erase(keyboard_input_message.key);

	if (keyboard_input_message.key == KeyboardKeys::ESCAPE)
	{
		PostMessage(_engine.GetMainWindowHandle(), WM_CLOSE, 0, 0);
	}
	else if (keyboard_input_message.key == KeyboardKeys::PAUSE)
	{
		if (mt::time::TimeManager& time_manager = *_engine.GetTimeManager(); time_manager.IsUpdatePaused())
		{
			time_manager.Continue();
		}
		else
		{
			time_manager.Pause();
		}
	}
}


void InputManager::KeyboardEvent(KeyboardKeys key, KeyState key_state)
{
	auto ptr = _message_pool.GetMemory();
	ptr->emplace<KeyboardInputMessage>(this, key, key_state);
	_input_queue.push(ptr);
}

void InputManager::MouseEvent(__int32 x, __int32 y, bool left_button, bool right_button, bool middle_button, bool button_4, bool button_5)
{
	auto ptr = _message_pool.GetMemory();
	ptr->emplace<MouseInputMessage>(this, x, y, left_button, right_button, middle_button, button_4, button_5);
	_input_queue.push(ptr);
}

//
//
//void mt::input::InputManager::MouseMove(WPARAM button_state, int x, int y)
//{
//	_input_queue.push(
//		reinterpret_cast<InputMessage*>(
//			new (_message_pool.GetMemory()) IM_MouseMove(_engine.GetInputManager(), x, y)
//		)
//	);
//}
//
//void mt::input::InputManager::MouseDown(WPARAM button_state, int x, int y)
//{	
//	SetCapture(_engine.GetMainWindowHandle());
//
//	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_MouseDown(this, button_state)));
//}
//
//void mt::input::InputManager::MouseUp(WPARAM button_state, int x, int y)
//{
//	ReleaseCapture();
//
//	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_MouseUp(this, button_state)));
//}
//
//// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646280(v=vs.85).aspx
//void mt::input::InputManager::KeyDown(WPARAM vk_key, LPARAM flags)
//{
//	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_KeyDown(this, vk_key)));
//}
//
//// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646281(v=vs.85).aspx
//void mt::input::InputManager::KeyUp(WPARAM vk_key, LPARAM flags)
//{
//	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_KeyUp(this, vk_key)));
//}
//
//void mt::input::InputManager::_MouseMove(__int32 x, __int32 y)
//{
//	// Left mouse button is being held
//	if (_held_keys_and_buttons.find(MK_LBUTTON) != _held_keys_and_buttons.end())
//	{
//		auto& camera = _engine.GetRenderer()->GetCurrentCamera();
//
//		// Make each pixel correspond to 1/10th of a degree.
//		float dx = DirectX::XMConvertToRadians(0.1f*static_cast<float>(x - _mouse_position.x));
//		float dy = DirectX::XMConvertToRadians(0.1f*static_cast<float>(y - _mouse_position.y));
//
//		camera.Pitch(dy);
//		camera.RotateY(dx);
//	}
//
//	_mouse_position.x = x;
//	_mouse_position.y = y;
//}
//
//void mt::input::InputManager::_MouseDown(WPARAM btnState)
//{
//	OutputDebugStringW((L"Down " + std::to_wstring(btnState) + L'\n').c_str());
//
//	_held_keys_and_buttons.insert(btnState);
//}
//
//void mt::input::InputManager::_MouseUp(WPARAM btnState)
//{
//	OutputDebugStringW((L"Up " + std::to_wstring(btnState) + L'\n').c_str());
//
//	_held_keys_and_buttons.erase(btnState);
//}
//
//void mt::input::InputManager::_KeyDown(WPARAM vk_key)
//{
//
//}
//
//void mt::input::InputManager::_KeyUp(WPARAM vk_key)
//{
//	if (vk_key == VK_ESCAPE)
//	{
//		PostMessage(_engine.GetMainWindowHandle(), WM_CLOSE, 0, 0);
//	}
//
//	// P Key
//	else if (vk_key == 0x50)
//	{
//		if (mt::time::TimeManager& time_manager = *_engine.GetTimeManager(); time_manager.IsUpdatePaused())
//		{
//			time_manager.Continue();
//		}
//		else
//		{
//			time_manager.Pause();
//		}
//	}
//}