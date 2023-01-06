module; 

#include <windows.h>

module WindowManager;

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

	
	_window_class.style = CS_HREDRAW | CS_VREDRAW;
	_window_class.lpfnWndProc = ::MainWndProc;
	_window_class.cbClsExtra = 0;
	_window_class.cbWndExtra = 0;
	_window_class.hInstance = _instance_handle;
	_window_class.hIcon = LoadIcon(0, IDI_APPLICATION);
	_window_class.hCursor = LoadCursor(0, IDC_ARROW);
	_window_class.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	_window_class.lpszMenuName = 0;
	_window_class.lpszClassName = L"MainWnd";

	if (!RegisterClass(&_window_class))
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
#pragma warning (push)
#pragma warning (disable: 4715)
std::expected<void, mt::Error> WindowManager::resize(int width, int height)
{
	// This flag should prevent further rendering after the current frame finishes
	setIsWindowResizing(true);

	// wait until rendering is finished.
	while (_engine.getRenderer()->getIsRendering()) {};

	if (auto expected = _engine.getRenderer()->resize(width, height); !expected)
		return std::unexpected(expected.error());
	
	// TODO: Callbacks?

	// Continue rendering.
	setIsWindowResizing(false);
}
#pragma warning (pop)