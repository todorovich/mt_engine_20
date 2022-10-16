// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module InputMessage;

import Engine;
import InputManager;

 void mt::input::InputMessage::_MouseMove(__int32 x, __int32 y)
{
	mt::Engine::GetInputManager()._MouseMove(x, y);
}

void mt::input::InputMessage::_MouseDown(WPARAM btnState)
{
	mt::Engine::GetInputManager()._MouseDown(btnState);
}

void mt::input::InputMessage::_MouseUp(WPARAM btnState)
{
	mt::Engine::GetInputManager()._MouseUp(btnState);
}

void mt::input::InputMessage::_KeyDown(WPARAM vk_key)
{
	mt::Engine::GetInputManager()._KeyDown(vk_key);
}

void mt::input::InputMessage::_KeyUp(WPARAM vk_key)
{
	mt::Engine::GetInputManager()._KeyUp(vk_key);
}
