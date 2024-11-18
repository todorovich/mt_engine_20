// Copyright 2024 Micho Todorovich, all rights reserved.
module;

#include <Windows.h>
#include <windowsx.h>

export module Windows;

namespace windows
{
	HWND _CreateWindowW(
		LPCWSTR   lpClassName,
		LPCWSTR   lpWindowName,
		DWORD     dwStyle,
		int       x,
		int       y,
		int       nWidth,
		int       nHeight,
		HWND      hWndParent,
		HMENU     hMenu,
		HINSTANCE hInstance,
		LPVOID    lpParam
	)
	{
		return CreateWindowW(
			lpClassName,
			lpWindowName,
			dwStyle,
			x,
			y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
		);
	}

	constexpr WORD _LOWORD(DWORD_PTR dwordPointer)
	{
		return LOWORD(dwordPointer);
	}

	constexpr WORD _HIWORD(DWORD_PTR dwordPointer)
	{
		return HIWORD(dwordPointer);
	}

	constexpr LRESULT _MAKELRESULT(DWORD_PTR l, DWORD_PTR h)
	{
		return MAKELRESULT(l, h);
	}

	constexpr int _GET_X_LPARAM(LPARAM lparam)
	{
		return GET_X_LPARAM(lparam);
	}
	
	constexpr int _GET_Y_LPARAM(LPARAM lparam)
	{
		return GET_Y_LPARAM(lparam);
	}
}

export namespace windows
{
	using ::HWND;
	using ::UINT;
	using ::PRAWINPUT;
	using ::HRAWINPUT;
	using ::RAWINPUT;
	using ::LRESULT;
	using ::HRESULT;
	using ::RAWINPUTHEADER;
	using ::LPBYTE;
	using ::BYTE;
	using ::WPARAM;
	using ::LPARAM;
	using ::LPCWSTR;
	using ::WORD;
	using ::DWORD;
	using ::MINMAXINFO;
	using ::MSG;
	using ::HINSTANCE;
	using ::RECT;
	using ::POINT;
	using ::WNDCLASS;
	using ::HBRUSH;
	using ::PSTR;
	using ::HANDLE;

	const LPWSTR IDC_ARROW_VALUE = IDC_ARROW;
#undef IDC_ARROW
	const LPWSTR IDC_ARROW = IDC_ARROW_VALUE;

	const LPWSTR IDI_APPLICATION_VALUE = IDI_APPLICATION;
#undef IDI_APPLICATION
	const LPWSTR IDI_APPLICATION = IDI_APPLICATION_VALUE;

	constexpr auto SM_CYSCREEN_VALUE = SM_CYSCREEN;
#undef SM_CYSCREEN
	constexpr auto SM_CYSCREEN = SM_CYSCREEN_VALUE;

	constexpr auto SM_CXSCREEN_VALUE = SM_CXSCREEN;
#undef SM_CXSCREEN
	constexpr auto SM_CXSCREEN = SM_CXSCREEN_VALUE;

	constexpr auto NULL_BRUSH_VALUE = NULL_BRUSH;
#undef NULL_BRUSH
	constexpr auto NULL_BRUSH = NULL_BRUSH_VALUE;

	constexpr auto CS_HREDRAW_VALUE = CS_HREDRAW;
#undef CS_HREDRAW
	constexpr auto CS_HREDRAW = CS_HREDRAW_VALUE;

	constexpr auto CS_VREDRAW_VALUE = CS_VREDRAW;
#undef CS_VREDRAW
	constexpr auto CS_VREDRAW = CS_VREDRAW_VALUE;

	constexpr auto SW_SHOWMAXIMIZED_VALUE = SW_SHOWMAXIMIZED;
#undef SW_SHOWMAXIMIZED
	constexpr auto SW_SHOWMAXIMIZED = SW_SHOWMAXIMIZED_VALUE;

	constexpr auto GWL_STYLE_VALUE = GWL_STYLE;
#undef GWL_STYLE
	constexpr auto GWL_STYLE = GWL_STYLE_VALUE;

	constexpr auto WS_MAXIMIZE_VALUE = WS_MAXIMIZE;
#undef WS_MAXIMIZE
	constexpr auto WS_MAXIMIZE = WS_MAXIMIZE_VALUE;

	constexpr auto PM_REMOVE_VALUE = PM_REMOVE;
#undef PM_REMOVE
	constexpr auto PM_REMOVE = PM_REMOVE_VALUE;

	constexpr auto SC_KEYMENU_VALUE = SC_KEYMENU;
#undef SC_KEYMENU
	constexpr auto SC_KEYMENU = SC_KEYMENU_VALUE;

	constexpr auto SIZE_MAXIMIZED_VALUE = SIZE_MAXIMIZED;
#undef SIZE_MAXIMIZED
	constexpr auto SIZE_MAXIMIZED = SIZE_MAXIMIZED_VALUE;

	constexpr auto SIZE_MINIMIZED_VALUE = SIZE_MINIMIZED;
#undef SIZE_MINIMIZED
	constexpr auto SIZE_MINIMIZED = SIZE_MINIMIZED_VALUE;

	constexpr auto SIZE_RESTORED_VALUE = SIZE_RESTORED;
#undef SIZE_RESTORED
	constexpr auto SIZE_RESTORED = SIZE_RESTORED_VALUE;

	constexpr auto MNC_CLOSE_VALUE = MNC_CLOSE;
#undef MNC_CLOSE
	constexpr auto MNC_CLOSE = MNC_CLOSE_VALUE;

	constexpr auto WA_INACTIVE_VALUE = WA_INACTIVE;
#undef WA_INACTIVE
	constexpr auto WA_INACTIVE = WA_INACTIVE_VALUE;

	constexpr auto RIM_TYPEKEYBOARD_VALUE = RIM_TYPEKEYBOARD;
#undef RIM_TYPEKEYBOARD
	constexpr auto RIM_TYPEKEYBOARD = RIM_TYPEKEYBOARD_VALUE;

	constexpr auto RID_INPUT_VALUE = RID_INPUT;
#undef RID_INPUT
	constexpr auto RID_INPUT = RID_INPUT_VALUE;

	constexpr auto NULL_VALUE = NULL;
#undef NULL
	constexpr auto NULL = NULL_VALUE;

	constexpr auto RIM_TYPEMOUSE_VALUE = RIM_TYPEMOUSE;
#undef RIM_TYPEMOUSE
	constexpr auto RIM_TYPEMOUSE = RIM_TYPEMOUSE_VALUE;
	
	

	auto& UnregisterClass_TEMP = UnregisterClass;
#undef UnregisterClass
	auto& UnregisterClass = UnregisterClass_TEMP;

	auto& RegisterClass_TEMP = RegisterClass;
#undef RegisterClass
	auto& RegisterClass = RegisterClass_TEMP;

	auto& LoadCursor_TEMP = LoadCursor;
#undef LoadCursor
	auto& LoadCursor = LoadCursor_TEMP;

	auto& LoadIcon_TEMP = LoadIcon;
#undef LoadIcon
	auto& LoadIcon = LoadIcon_TEMP;

	auto& GetModuleHandle_TEMP = GetModuleHandle;
#undef GetModuleHandle
	auto& GetModuleHandle = GetModuleHandle_TEMP;

	auto& DispatchMessage_TEMP = DispatchMessage;
#undef DispatchMessage
	auto& DispatchMessage = DispatchMessage_TEMP;

	auto& PeekMessage_TEMP = PeekMessage;
#undef PeekMessage
	auto& PeekMessage = PeekMessage_TEMP;

	auto& OutputDebugString_TEMP = OutputDebugString;
#undef OutputDebugString
	auto& OutputDebugString = OutputDebugString_TEMP;

	auto& DefWindowProc_TEMP = DefWindowProc;
#undef DefWindowProc
	auto& DefWindowProc = DefWindowProc_TEMP;

	using ::DefRawInputProc;
	using ::GetRawInputData;
	using ::PostQuitMessage;
	using ::SetThreadDescription;
	using ::TranslateMessage;
	using ::DestroyWindow;
	using ::GetLastError;
	using ::ShowCursor;
	using ::GetCurrentThread;
	using ::GetCursorPos;
	using ::SetCursorPos;
	using ::SetWindowLongPtrW;
	using ::ShowWindow;
	using ::GetStockObject;
	using ::GetSystemMetrics;
	
#undef CreateWindowW
	HWND CreateWindowW(
		LPCWSTR   lpClassName,
		LPCWSTR   lpWindowName,
		DWORD     dwStyle,
		int       x,
		int       y,
		int       nWidth,
		int       nHeight,
		HWND      hWndParent,
		HMENU     hMenu,
		HINSTANCE hInstance,
		LPVOID    lpParam
	)
	{
		return _CreateWindowW(
			lpClassName,
			lpWindowName,
			dwStyle,
			x,
			y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
		);
	}

#undef FAILED
	constexpr bool FAILED(HRESULT hr)
	{
		return hr < 0;
	}

#undef LOWORD
	constexpr WORD LOWORD(DWORD_PTR dwordPointer)
	{
		return _LOWORD(dwordPointer);
	}

#undef HIWORD
	constexpr WORD HIWORD(DWORD_PTR dwordPointer)
	{
		return _HIWORD(dwordPointer);
	}

#undef MAKELRESULT
	constexpr LRESULT MAKELRESULT(DWORD_PTR l, DWORD_PTR h)
	{
		return _MAKELRESULT(l, h);
	}

#undef GET_X_LPARAM
	constexpr int GET_X_LPARAM(LPARAM lparam)
	{
		return _GET_X_LPARAM(lparam);
	}

#undef GET_Y_LPARAM
	constexpr int GET_Y_LPARAM(LPARAM lparam)
	{
		return _GET_Y_LPARAM(lparam);
	}
}

// Windows Message MACROs to constexpr
export namespace windows
{
	constexpr auto WM_ACTIVATE_VALUE = WM_ACTIVATE;
#undef WM_ACTIVATE
	constexpr auto WM_ACTIVATE = WM_ACTIVATE_VALUE;

	constexpr auto WM_CAPTURECHANGED_VALUE = WM_CAPTURECHANGED;
#undef WM_CAPTURECHANGED
	constexpr auto WM_CAPTURECHANGED = WM_CAPTURECHANGED_VALUE;

	constexpr auto WM_CLOSE_VALUE = WM_CLOSE;
#undef WM_CLOSE
	constexpr auto WM_CLOSE = WM_CLOSE_VALUE;

	constexpr auto WM_DESTROY_VALUE = WM_DESTROY;
#undef WM_DESTROY
	constexpr auto WM_DESTROY = WM_DESTROY_VALUE;

	constexpr auto WM_ENTERSIZEMOVE_VALUE = WM_ENTERSIZEMOVE;
#undef WM_ENTERSIZEMOVE
	constexpr auto WM_ENTERSIZEMOVE = WM_ENTERSIZEMOVE_VALUE;

	constexpr auto WM_ERASEBKGND_VALUE = WM_ERASEBKGND;
#undef WM_ERASEBKGND
	constexpr auto WM_ERASEBKGND = WM_ERASEBKGND_VALUE;

	constexpr auto WM_EXITSIZEMOVE_VALUE = WM_EXITSIZEMOVE;
#undef WM_EXITSIZEMOVE
	constexpr auto WM_EXITSIZEMOVE = WM_EXITSIZEMOVE_VALUE;

	constexpr auto WM_GETICON_VALUE = WM_GETICON;
#undef WM_GETICON
	constexpr auto WM_GETICON = WM_GETICON_VALUE;

	constexpr auto WM_GETMINMAXINFO_VALUE = WM_GETMINMAXINFO;
#undef WM_GETMINMAXINFO
	constexpr auto WM_GETMINMAXINFO = WM_GETMINMAXINFO_VALUE;

	constexpr auto WM_INPUT_VALUE = WM_INPUT;
#undef WM_INPUT
	constexpr auto WM_INPUT = WM_INPUT_VALUE;

	constexpr auto WM_KEYDOWN_VALUE = WM_KEYDOWN;
#undef WM_KEYDOWN
	constexpr auto WM_KEYDOWN = WM_KEYDOWN_VALUE;

	constexpr auto WM_KEYUP_VALUE = WM_KEYUP;
#undef WM_KEYUP
	constexpr auto WM_KEYUP = WM_KEYUP_VALUE;

	constexpr auto WM_LBUTTONDOWN_VALUE = WM_LBUTTONDOWN;
#undef WM_LBUTTONDOWN
	constexpr auto WM_LBUTTONDOWN = WM_LBUTTONDOWN_VALUE;

	constexpr auto WM_LBUTTONUP_VALUE = WM_LBUTTONUP;
#undef WM_LBUTTONUP
	constexpr auto WM_LBUTTONUP = WM_LBUTTONUP_VALUE;

	constexpr auto WM_MBUTTONDOWN_VALUE = WM_MBUTTONDOWN;
#undef WM_MBUTTONDOWN
	constexpr auto WM_MBUTTONDOWN = WM_MBUTTONDOWN_VALUE;

	constexpr auto WM_MBUTTONUP_VALUE = WM_MBUTTONUP;
#undef WM_MBUTTONUP
	constexpr auto WM_MBUTTONUP = WM_MBUTTONUP_VALUE;

	constexpr auto WM_MENUCHAR_VALUE = WM_MENUCHAR;
#undef WM_MENUCHAR
	constexpr auto WM_MENUCHAR = WM_MENUCHAR_VALUE;

	constexpr auto WM_MOUSEMOVE_VALUE = WM_MOUSEMOVE;
#undef WM_MOUSEMOVE
	constexpr auto WM_MOUSEMOVE = WM_MOUSEMOVE_VALUE;

	constexpr auto WM_MOUSEWHEEL_VALUE = WM_MOUSEWHEEL;
#undef WM_MOUSEWHEEL
	constexpr auto WM_MOUSEWHEEL = WM_MOUSEWHEEL_VALUE;

	constexpr auto WM_MOVE_VALUE = WM_MOVE;
#undef WM_MOVE
	constexpr auto WM_MOVE = WM_MOVE_VALUE;

	constexpr auto WM_MOVING_VALUE = WM_MOVING;
#undef WM_MOVING
	constexpr auto WM_MOVING = WM_MOVING_VALUE;

	constexpr auto WM_NCCALCSIZE_VALUE = WM_NCCALCSIZE;
#undef WM_NCCALCSIZE
	constexpr auto WM_NCCALCSIZE = WM_NCCALCSIZE_VALUE;

	constexpr auto WM_NCMOUSELEAVE_VALUE = WM_NCMOUSELEAVE;
#undef WM_NCMOUSELEAVE
	constexpr auto WM_NCMOUSELEAVE = WM_NCMOUSELEAVE_VALUE;

	constexpr auto WM_NCPAINT_VALUE = WM_NCPAINT;
#undef WM_NCPAINT
	constexpr auto WM_NCPAINT = WM_NCPAINT_VALUE;

	constexpr auto WM_PAINT_VALUE = WM_PAINT;
#undef WM_PAINT
	constexpr auto WM_PAINT = WM_PAINT_VALUE;

	constexpr auto WM_QUIT_VALUE = WM_QUIT;
#undef WM_QUIT
	constexpr auto WM_QUIT = WM_QUIT_VALUE;

	constexpr auto WM_RBUTTONDOWN_VALUE = WM_RBUTTONDOWN;
#undef WM_RBUTTONDOWN
	constexpr auto WM_RBUTTONDOWN = WM_RBUTTONDOWN_VALUE;

	constexpr auto WM_RBUTTONUP_VALUE = WM_RBUTTONUP;
#undef WM_RBUTTONUP
	constexpr auto WM_RBUTTONUP = WM_RBUTTONUP_VALUE;

	constexpr auto WM_SETTEXT_VALUE = WM_SETTEXT;
#undef WM_SETTEXT
	constexpr auto WM_SETTEXT = WM_SETTEXT_VALUE;

	constexpr auto WM_SIZE_VALUE = WM_SIZE;
#undef WM_SIZE
	constexpr auto WM_SIZE = WM_SIZE_VALUE;

	constexpr auto WM_SIZING_VALUE = WM_SIZING;
#undef WM_SIZING
	constexpr auto WM_SIZING = WM_SIZING_VALUE;

	constexpr auto WM_SYSCOMMAND_VALUE = WM_SYSCOMMAND;
#undef WM_SYSCOMMAND
	constexpr auto WM_SYSCOMMAND = WM_SYSCOMMAND_VALUE;

	constexpr auto WM_SYSKEYDOWN_VALUE = WM_SYSKEYDOWN;
#undef WM_SYSKEYDOWN
	constexpr auto WM_SYSKEYDOWN = WM_SYSKEYDOWN_VALUE;

	constexpr auto WM_SYSKEYUP_VALUE = WM_SYSKEYUP;
#undef WM_SYSKEYUP
	constexpr auto WM_SYSKEYUP = WM_SYSKEYUP_VALUE;

	constexpr auto WM_TIMER_VALUE = WM_TIMER;
#undef WM_TIMER
	constexpr auto WM_TIMER = WM_TIMER_VALUE;

	constexpr auto WM_XBUTTONDOWN_VALUE = WM_XBUTTONDOWN;
#undef WM_XBUTTONDOWN
	constexpr auto WM_XBUTTONDOWN = WM_XBUTTONDOWN_VALUE;

	constexpr auto WM_XBUTTONUP_VALUE = WM_XBUTTONUP;
#undef WM_XBUTTONUP
	constexpr auto WM_XBUTTONUP = WM_XBUTTONUP_VALUE;
}