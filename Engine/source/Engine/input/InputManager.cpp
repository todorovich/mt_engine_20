// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <DirectXMath.h>
#include <windows.h>

module InputManager;

import IM_MouseMove;
import IM_MouseDown;
import IM_MouseUp;
import IM_KeyDown;
import IM_KeyUp;

import Engine;

void mt::input::InputManager::ProcessInput()
{
	auto size = _input_queue.size();
	for (auto x = 0u; x < size; x++)
	{
		InputMessage* input_message = _input_queue.front();
		_input_queue.pop();
		input_message->Execute();
		_message_pool.ReleaseMemory(input_message);
	}
}

void mt::input::InputManager::MouseMove(WPARAM button_state, int x, int y)
{
	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_MouseMove(x, y)));
}

void mt::input::InputManager::MouseDown(WPARAM button_state, int x, int y)
{	
	SetCapture(Engine::GetMainWindowHandle());

	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_MouseDown(button_state)));
}

void mt::input::InputManager::MouseUp(WPARAM button_state, int x, int y)
{
	ReleaseCapture();

	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_MouseUp(button_state)));
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646280(v=vs.85).aspx
void mt::input::InputManager::KeyDown(WPARAM vk_key, LPARAM flags)
{
	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_KeyDown(vk_key)));
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646281(v=vs.85).aspx
void mt::input::InputManager::KeyUp(WPARAM vk_key, LPARAM flags)
{
	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_KeyUp(vk_key)));
}

void mt::input::InputManager::_MouseMove(__int32 x, __int32 y)
{
	// Left mouse button is being held
	if (_held_keys_and_buttons.find(MK_LBUTTON) != _held_keys_and_buttons.end())
	{
		auto& camera = Engine::GetCurrentCamera();

		// Make each pixel correspond to 1/10th of a degree.
		float dx = DirectX::XMConvertToRadians(0.1f*static_cast<float>(x - _mouse_position.x));
		float dy = DirectX::XMConvertToRadians(0.1f*static_cast<float>(y - _mouse_position.y));

		camera.Pitch(dy);
		camera.RotateY(dx);
	}

	_mouse_position.x = x;
	_mouse_position.y = y;
}

void mt::input::InputManager::_MouseDown(WPARAM btnState)
{
	_held_keys_and_buttons.insert(btnState);
}

void mt::input::InputManager::_MouseUp(WPARAM btnState)
{
	_held_keys_and_buttons.erase(btnState);
}

void mt::input::InputManager::_KeyDown(WPARAM vk_key)
{

}

void mt::input::InputManager::_KeyUp(WPARAM vk_key)
{
	if (vk_key == VK_ESCAPE)
	{
		PostMessage(mt::Engine::GetMainWindowHandle(), WM_CLOSE, 0, 0);
	}

	// P Key
	else if (vk_key == 0x50)
	{
		if (mt::Engine::GetTimeManager().IsUpdatePaused())
		{
			mt::Engine::GetTimeManager().Continue();
		}
		else
		{
			mt::Engine::GetTimeManager().Pause();
		}
	}
}