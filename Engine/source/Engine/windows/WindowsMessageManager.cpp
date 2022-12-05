// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessageManager;

import WindowsMessages.Activate;
import WindowsMessages.CaptureChanged;
import WindowsMessages.Close;
import WindowsMessages.destroy;
import WindowsMessages.EnterSizeMove;
import WindowsMessages.EraseBackground;
import WindowsMessages.ExitSizeMove;
import WindowsMessages.GetIcon;
import WindowsMessages.GetMinMaxInfo;
import WindowsMessages.Input;
import WindowsMessages.KeyDown;
import WindowsMessages.KeyUp;
import WindowsMessages.LeftMouseButtonDown;
import WindowsMessages.LeftMouseButtonUp;
import WindowsMessages.MiddleMouseButtonDown;
import WindowsMessages.MiddleMouseButtonUp;
import WindowsMessages.MenuChar;
import WindowsMessages.MouseMove;
import WindowsMessages.MouseWheel;
import WindowsMessages.Move;
import WindowsMessages.Moving;
import WindowsMessages.NonClientCalcSize;
import WindowsMessages.NonClientMouseLeave;
import WindowsMessages.NonClientPaint;
import WindowsMessages.Paint;
import WindowsMessages.Quit;
import WindowsMessages.RightMouseButtonDown;
import WindowsMessages.RightMouseButtonUp;
import WindowsMessages.SetText;
import WindowsMessages.Size;
import WindowsMessages.Sizing;
import WindowsMessages.SystemCommand;
import WindowsMessages.SystemKeyDown;
import WindowsMessages.SystemKeyUp;
import WindowsMessages.Timer;
import WindowsMessages.XButtonDown;
import WindowsMessages.XButtonUp;
//import WindowsMessages.WindowPositionChanging;

import Engine;
import std.core;

using namespace mt::windows;

WindowsMessageManager::WindowsMessageManager(mt::Engine& engine)
	: _engine(engine)
{}

mt::windows::WindowsMessageManager::~WindowsMessageManager()
{
	// clean up all those message handlers
	for (auto& pair : _message_handler_map)
	{
		delete pair.second;
	}
}

void WindowsMessageManager::initialize()
{
	// Add all the message handlers to the message handler map
	_message_handler_map.insert(std::make_pair(WM_ACTIVATE, new WM_Activate(_engine.getTimeManager(), _engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_CAPTURECHANGED, new WM_CaptureChanged()));
	_message_handler_map.insert(std::make_pair(WM_CLOSE, new WM_Close(_engine)));
	_message_handler_map.insert(std::make_pair(WM_DESTROY, new WM_Destroy()));
	_message_handler_map.insert(std::make_pair(WM_ENTERSIZEMOVE, new WM_EnterSizeMove(_engine)));
	_message_handler_map.insert(std::make_pair(WM_ERASEBKGND, new WM_EraseBackground()));
	_message_handler_map.insert(std::make_pair(WM_EXITSIZEMOVE, new WM_ExitSizeMove(_engine)));
	_message_handler_map.insert(std::make_pair(WM_GETICON, new WM_GetIcon()));
	_message_handler_map.insert(std::make_pair(WM_GETMINMAXINFO, new WM_GetMinMaxInfo()));
	_message_handler_map.insert(std::make_pair(WM_INPUT, new WM_Input()));
	_message_handler_map.insert(std::make_pair(WM_KEYDOWN, new WM_KeyDown(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_KEYUP, new WM_KeyUp(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_LBUTTONDOWN, new WM_LeftMouseButtonDown(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_LBUTTONUP, new WM_LeftMouseButtonUp(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_MBUTTONDOWN, new WM_MiddleMouseButtonDown(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_MBUTTONUP, new WM_MiddleMouseButtonUp(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_MENUCHAR, new WM_MenuChar()));
	_message_handler_map.insert(std::make_pair(WM_MOUSEMOVE, new WM_MouseMove(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_MOUSEWHEEL, new WM_MouseWheel(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_MOVE, new WM_Move()));
	_message_handler_map.insert(std::make_pair(WM_MOVING, new WM_Moving()));
	_message_handler_map.insert(std::make_pair(WM_NCCALCSIZE, new WM_NonClientCalcSize()));
	_message_handler_map.insert(std::make_pair(WM_NCMOUSELEAVE, new WM_NonClientMouseLeave()));
	_message_handler_map.insert(std::make_pair(WM_NCPAINT, new WM_NonClientPaint()));
	_message_handler_map.insert(std::make_pair(WM_PAINT, new WM_Paint()));
	_message_handler_map.insert(std::make_pair(WM_QUIT, new WM_Quit()));
	_message_handler_map.insert(std::make_pair(WM_RBUTTONDOWN, new WM_RightMouseButtonDown(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_RBUTTONUP, new WM_RightMouseButtonUp(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_SETTEXT, new WM_SetText()));
	_message_handler_map.insert(std::make_pair(WM_SIZE, new WM_Size(_engine)));
	_message_handler_map.insert(std::make_pair(WM_SIZING, new WM_Sizing()));
	_message_handler_map.insert(std::make_pair(WM_SYSCOMMAND, new WM_SystemCommand()));
	_message_handler_map.insert(std::make_pair(WM_SYSKEYDOWN, new WM_SystemKeyDown(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_SYSKEYUP, new WM_SystemKeyUp(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_TIMER, new WM_Timer()));
	_message_handler_map.insert(std::make_pair(WM_XBUTTONDOWN, new WM_XButtonDown(_engine.getInputManager())));
	_message_handler_map.insert(std::make_pair(WM_XBUTTONUP, new WM_XButtonUp(_engine.getInputManager())));
	//_message_handler_map.insert(std::make_pair(WM_WINDOWPOSCHANGING, new WM_WindowPositionChanging())); // WTF? Why does it hate this one?
}

LRESULT mt::windows::WindowsMessageManager::handle_message(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto it = _message_handler_map.find(msg);

	if (it != _message_handler_map.end())
	{
		auto result = (*it).second->execute(hwnd, msg, wParam, lParam);

		return result;
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

