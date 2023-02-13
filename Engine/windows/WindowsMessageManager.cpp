// Copyright 2023 Micho Todorovich, all rights reserved.
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

using namespace mt::windows;
using namespace mt::error;


WindowsMessageManager::WindowsMessageManager(mt::Engine& engine, Error& error)
	: WindowsMessageManagerInterface(error)
	, _windows_message_loop_task(engine)
	, _engine(engine)
{
	_message_handler_map.insert(std::make_pair(WM_ACTIVATE, std::make_unique<WM_Activate>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_CAPTURECHANGED, std::make_unique<WM_CaptureChanged>()));
	_message_handler_map.insert(std::make_pair(WM_CLOSE, std::make_unique<WM_Close>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_DESTROY, std::make_unique<WM_Destroy>()));
	_message_handler_map.insert(std::make_pair(WM_ENTERSIZEMOVE, std::make_unique<WM_EnterSizeMove>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_ERASEBKGND, std::make_unique<WM_EraseBackground>()));
	_message_handler_map.insert(std::make_pair(WM_EXITSIZEMOVE, std::make_unique<WM_ExitSizeMove>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_GETICON, std::make_unique<WM_GetIcon>()));
	_message_handler_map.insert(std::make_pair(WM_GETMINMAXINFO, std::make_unique<WM_GetMinMaxInfo>()));
	_message_handler_map.insert(std::make_pair(WM_INPUT, std::make_unique<WM_Input>()));
	_message_handler_map.insert(std::make_pair(WM_KEYDOWN, std::make_unique<WM_KeyDown>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_KEYUP, std::make_unique<WM_KeyUp>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_LBUTTONDOWN, std::make_unique<WM_LeftMouseButtonDown>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_LBUTTONUP, std::make_unique<WM_LeftMouseButtonUp>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_MBUTTONDOWN, std::make_unique<WM_MiddleMouseButtonDown>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_MBUTTONUP, std::make_unique<WM_MiddleMouseButtonUp>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_MENUCHAR, std::make_unique<WM_MenuChar>()));
	_message_handler_map.insert(std::make_pair(WM_MOUSEMOVE, std::make_unique<WM_MouseMove>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_MOUSEWHEEL, std::make_unique<WM_MouseWheel>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_MOVE, std::make_unique<WM_Move>()));
	_message_handler_map.insert(std::make_pair(WM_MOVING, std::make_unique<WM_Moving>()));
	_message_handler_map.insert(std::make_pair(WM_NCCALCSIZE, std::make_unique<WM_NonClientCalcSize>()));
	_message_handler_map.insert(std::make_pair(WM_NCMOUSELEAVE, std::make_unique<WM_NonClientMouseLeave>()));
	_message_handler_map.insert(std::make_pair(WM_NCPAINT, std::make_unique<WM_NonClientPaint>()));
	_message_handler_map.insert(std::make_pair(WM_PAINT, std::make_unique<WM_Paint>()));
	_message_handler_map.insert(std::make_pair(WM_QUIT, std::make_unique<WM_Quit>()));
	_message_handler_map.insert(std::make_pair(WM_RBUTTONDOWN, std::make_unique<WM_RightMouseButtonDown>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_RBUTTONUP, std::make_unique<WM_RightMouseButtonUp>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_SETTEXT, std::make_unique<WM_SetText>()));
	_message_handler_map.insert(std::make_pair(WM_SIZE, std::make_unique<WM_Size>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_SIZING, std::make_unique<WM_Sizing>()));
	_message_handler_map.insert(std::make_pair(WM_SYSCOMMAND, std::make_unique<WM_SystemCommand>()));
	_message_handler_map.insert(std::make_pair(WM_SYSKEYDOWN, std::make_unique<WM_SystemKeyDown>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_SYSKEYUP, std::make_unique<WM_SystemKeyUp>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_TIMER, std::make_unique<WM_Timer>()));
	_message_handler_map.insert(std::make_pair(WM_XBUTTONDOWN, std::make_unique<WM_XButtonDown>(&_engine)));
	_message_handler_map.insert(std::make_pair(WM_XBUTTONUP, std::make_unique<WM_XButtonUp>(&_engine)));
	//_message_handler_map.insert(std::make_pair(WM_WINDOWPOSCHANGING, new WM_WindowPositionChanging())); // WTF? Why does it hate this one?
}

void mt::windows::WindowsMessageManager::destroyMainWindow() noexcept
{
	_windows_message_loop_task.destroyMainWindow();
}

bool mt::windows::WindowsMessageManager::hasReceivedQuit() noexcept
{
	return _windows_message_loop_task.hasReceivedQuit();
}

void mt::windows::WindowsMessageManager::toggleShowCursor() noexcept
{
	return _windows_message_loop_task.toggleShowCursor();
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
