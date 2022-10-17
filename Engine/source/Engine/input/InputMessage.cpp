// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module InputMessage;

import Engine;
import InputManager;

 void mt::input::InputMessage::_MouseMove(__int32 x, __int32 y)
{
	_input_manager->_MouseMove(x, y);
}

void mt::input::InputMessage::_MouseDown(WPARAM btnState)
{
	_input_manager->_MouseDown(btnState);
}

void mt::input::InputMessage::_MouseUp(WPARAM btnState)
{
	_input_manager->_MouseUp(btnState);
}

void mt::input::InputMessage::_KeyDown(WPARAM vk_key)
{
	_input_manager->_KeyDown(vk_key);
}

void mt::input::InputMessage::_KeyUp(WPARAM vk_key)
{
	_input_manager->_KeyUp(vk_key);
}
