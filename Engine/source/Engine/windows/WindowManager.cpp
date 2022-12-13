module; 

#include <windows.h>

#pragma warning( push )
#pragma warning( disable : 5050 )
module WindowManager;

import std.core;
//import std.threading;
#pragma warning( pop )


import Engine;
import DirectXRenderer;
import WindowsMessageManager;

using namespace mt::windows;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return mt::Engine::_instance->_windows_message_manager->handle_message(hwnd, msg, wParam, lParam);
}

bool WindowManager::initializeMainWindow(HINSTANCE instance_handle)
{
	_instance_handle = instance_handle;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ::MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _instance_handle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT rectangle = { 0, 0, _engine.getRenderer()->getWindowWidth(), _engine.getRenderer()->getWindowHeight() };
	AdjustWindowRect(&rectangle, WS_OVERLAPPEDWINDOW, false);
	int width = rectangle.right - rectangle.left;
	int height = rectangle.bottom - rectangle.top;

	_main_window_handle = CreateWindow(
		L"MainWnd", _main_window_caption.c_str(), WS_MAXIMIZE, 0, 0, 
		width, height, nullptr, nullptr, _instance_handle, 0
	);

	if (!_main_window_handle)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}


	SetWindowLong(_main_window_handle, GWL_STYLE, 0); //remove all window styles, check MSDN for details

	ShowWindow(_main_window_handle, SW_SHOWMAXIMIZED); //display window

	UpdateWindow(_main_window_handle);

	return true;
}

// TODO: window manager.
void WindowManager::resize(int width, int height)
{
	// This flag should prevent futher rendering after the current frame finishes
	setIsWindowResizing(true);

	// wait until rendering is finished.
	while (_engine.getRenderer()->getIsRendering()) {};

	_engine.getRenderer()->resize(width, height);
	
	// TODO: Callbacks?

	// Continue rendering.
	setIsWindowResizing(false);
}